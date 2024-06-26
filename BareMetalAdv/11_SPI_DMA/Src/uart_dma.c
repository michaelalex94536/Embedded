/*
 * uart_dma.c
 *
 *  Created on: May 3, 2024
 *      Author: michael
 */

#include "uart_dma.h"

// To enable assorted clocks
#define UART2EN		(1U << 17) // UART2 clock enable
#define GPIOAEN		(1U << 0)  // GPIOA clock enable

// To put UART into Tx (TE) or Rx (RE) mode, and enable it (UE)
#define CR1_TE		(1U << 3)
#define CR1_RE		(1U << 2)
#define CR1_UE		(1U << 13)
#define SR_TXE		(1U << 7) //Transmit data register is empty

#define SR_TC		(1U << 6) //Used to clear TC flag
#define CR1_TCIE	(1U << 6)

// Needed to set the USART baudrate examples
#define UART_BAUDRATE	115200
#define CLK				16000000  // Default clock for this chip

#define CR3_DMAR		(1U << 6)
#define CR3_DMAT		(1U << 7)

#define DMA1EN			(1U << 21)
#define DMA_SCR_EN		(1U << 0)
#define DMA_SCR_MINC	(1U << 10)
#define DMA_SCR_PINC	(1U << 9)
#define DMA_SCR_CIRC	(1U << 8)
#define DMA_SCR_TCIE	(1U << 4)
#define DMA_SCR_TEIE	(1U << 2)
#define DMA_SFCR_DMDIS	(1U << 2)

#define HIFCR_CDMEIF5	(1U << 8)
#define HIFCR_CTEIF5	(1U << 9)
#define HIFCR_CHTIF5	(1U << 10)
#define HIFCR_CTCIF5	(1U << 11)

#define HIFCR_CDMEIF6	(1U << 18)
#define HIFCR_CTEIF6	(1U << 19)
#define HIFCR_CHTIF6	(1U << 20)
#define HIFCR_CTCIF6	(1U << 21)

#define HIFSR_TCIF5		(1U << 11)
#define HIFSR_TCIF6		(1U << 21)

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);

char uart_data_buffer[UART_DATA_BUF_SIZE];

uint8_t g_rx_cmplt;
uint8_t g_tx_cmplt;
uint8_t g_uart_cmplt;


void uart2_rx_tx_init(void)
{
	/*********************  Configure UART GPIO pin ********************/
	// 1) Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// 2) Set PA2 to alternate function (AF) mode
	GPIOA->MODER &= ~(1U << 4);  // Set bit 4 to 0
	GPIOA->MODER |= (1U << 5);   // Set bit 5 to 1

	// 3) Set PA3 to alternate function (AF) mode
	GPIOA->MODER &= ~(1U << 6);  // Set bit 6 to 0
	GPIOA->MODER |= (1U << 7);   // Set bit 7 to 1

	// 4) Set PA2 alternate function type to AF7 (USART2_TX)
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] |= (1U << 9);
	GPIOA->AFR[0] |= (1U << 10);
	GPIOA->AFR[0] &= ~(1U << 11);

	// 5) Set PA3 alternate function type to AF7 (USART2_RX)
	GPIOA->AFR[0] |= (1U << 12);
	GPIOA->AFR[0] |= (1U << 13);
	GPIOA->AFR[0] |= (1U << 14);
	GPIOA->AFR[0] &= ~(1U << 15);

	/*******************  Configure UART module  *********************/
	// 6) Enable clock access to UART2
	RCC->APB1ENR |= UART2EN;

	// 7) Set baudrate
	uart_set_baudrate(CLK, UART_BAUDRATE);

	// 8) Specify using DMA for both Rx and Tx and reset all other bits in CR3
	USART2->CR3 = (CR3_DMAR | CR3_DMAT);

	// 9) Set transfer direction to both Rx and Tx
	USART2->CR1 = (CR1_TE | CR1_RE);

	//10) Clear TC (transfer complete) flag
	USART2->SR &= ~SR_TC;

	//11) Enable TCIE (transfer complete interrupt)
	USART2->CR1 |= CR1_TCIE;

	//12) Enable UART module
	USART2->CR1 |= CR1_UE;

	/*13.Enable USART2 interrupt in the NVIC*/
	 NVIC_EnableIRQ(USART2_IRQn);
}

void dma1_init(void)
{
	// Enable clock access to DMA1 on AHB1
	RCC->AHB1ENR |= DMA1EN;

	// Enable DMA stream 6 interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

void dma1_stream5_uart_rx_config(void)
{
	// Disable DMA stream
	DMA1_Stream5->CR &= ~DMA_SCR_EN;

	// Wait until DMA is disabled
	while((DMA1_Stream5->CR & ~DMA_SCR_EN)){}

	// Clear interrupt flags for stream 5
	DMA1->HIFCR |= HIFCR_CDMEIF5;
	DMA1->HIFCR |= HIFCR_CTEIF5;
	DMA1->HIFCR |= HIFCR_CHTIF5;
	DMA1->HIFCR |= HIFCR_CTCIF5;

	// Set peripheral address
	DMA1_Stream5->PAR = (uint32_t)(&(USART2->DR));

	// Set memory address
	DMA1_Stream5->M0AR = (uint32_t)(&uart_data_buffer);

	// Set number of transfers
	DMA1_Stream5->NDTR = (uint16_t)UART_DATA_BUF_SIZE;

	// Select Channel 4
	DMA1_Stream5->CR &= ~(1U << 25);
	DMA1_Stream5->CR &= ~(1U << 26);
	DMA1_Stream5->CR |= (1U << 27);

	// Enable memory address increment
	DMA1_Stream5->CR |= DMA_SCR_MINC;

	// Enable transfer complete interrupt
	DMA1_Stream5->CR |= DMA_SCR_TCIE;

	// Enable circular mode
	DMA1_Stream5->CR |= DMA_SCR_CIRC;

	// Set transfer direction for Rx: peripheral to memory
	DMA1_Stream5->CR &= ~(1U << 6);
	DMA1_Stream5->CR &= ~(1U << 7);

	// Enable DMA stream
	DMA1_Stream5->CR |= DMA_SCR_EN;

	// Enable DMA Stream5 interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

void dma1_stream6_uart_tx_config(uint32_t msg_to_send, uint32_t msg_len)
{
	// Disable DMA stream
	DMA1_Stream6->CR &= ~DMA_SCR_EN;

	// Wait until DMA is disabled
	while(DMA1_Stream6->CR &= ~DMA_SCR_EN){}

	// Clear interrupt flags for stream 6
	DMA1->HIFCR |= HIFCR_CDMEIF6;
	DMA1->HIFCR |= HIFCR_CTEIF6;
	DMA1->HIFCR |= HIFCR_CHTIF6;
	DMA1->HIFCR |= HIFCR_CTCIF6;

	// Set peripheral address
	DMA1_Stream6->PAR = (uint32_t)(&(USART2->DR));

	// Set memory address
	DMA1_Stream6->M0AR = msg_to_send;

	// Set number of transfers
	DMA1_Stream6->NDTR = msg_len;

	// Select Channel 4
	DMA1_Stream6->CR &= ~(1U << 25);
	DMA1_Stream6->CR &= ~(1U << 26);
	DMA1_Stream6->CR |= (1U << 27);

	// Enable memory address increment
	DMA1_Stream6->CR |= DMA_SCR_MINC;

	// Set transfer direction for Tx: memory to peripheral
	DMA1_Stream6->CR |= (1U << 6);
	DMA1_Stream6->CR &= ~(1U << 7);

	// Set transfer complete interrupt
	DMA1_Stream6->CR |= DMA_SCR_TCIE;

	// Enable DMA stream
	DMA1_Stream6->CR |= DMA_SCR_EN;
}


// Function to compute baudrate
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U) )/baudrate);
}

// Function to set baudrate register
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}


void DMA1_Stream6_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF6)
	{
		// Do something here ...
		g_tx_cmplt = 1;

		// clear the flag
		DMA1->HISR |= HIFSR_TCIF6;
	}

}

void DMA1_Stream5_IRQHandler(void)
{
	if((DMA1->HISR) & HIFSR_TCIF5)
	{

		g_rx_cmplt = 1;
		// Do something here ...

		// clear the flag
		DMA1->HISR |= HIFSR_TCIF5;
	}

}

void USART2_IRQHandler(void)
{
	g_uart_cmplt = 1;

	// Clear TC Interrupt flag by writing a 0 to the register
	USART2->SR &= ~SR_TC;

}






































