/*
 * spi_dma.c
 *
 *  Created on: May 8, 2024
 *      Author: michael
 *
 *      PA5:   	SPI1_SCK
 *      PA6:	SPI1_MISO
 *      PA7:	SPI1_MOSI
 *
 *      DMA2 Channel 3, Stream 3 is SPI1_TX
 *      DMA2 Channel 3, Stream 2 is SPI1_RX
 *
 */

#include "spi_dma.h"


#define GPIOAEN			(1U << 0)
#define SPI1EN			(1U << 12)

#define CR1_SSM			(1U << 9)
#define CR1_SSI			(1U << 8)
#define CR1_MSTR		(1U << 2)
#define CR1_CPHA		(1U << 0)
#define CR1_CPOL		(1U << 1)
#define CR1_SPE			(1U << 6)

#define CR2_TXDMAEN		(1U << 1)
#define CR2_RXDMAEN		(1U << 0)

#define DMA2EN			(1U << 22)
#define DMA_SCR_EN		(1U << 0)
#define DMA_SCR_MINC	(1U << 10)
#define DMA_SCR_PINC	(1U << 9)
#define DMA_SCR_CIRC	(1U << 8)
#define DMA_SCR_TCIE	(1U << 4)
#define DMA_SCR_TEIE	(1U << 2)
#define DMA_SFCR_DMDIS	(1U << 2)

#define LIFCR_CDMEIF2	(1U << 18)
#define LIFCR_CTEIF2	(1U << 19)
#define LIFCR_CHTIF2	(1U << 20)
#define LIFCR_CTCIF2	(1U << 21)

#define LIFCR_CDMEIF3	(1U << 24)
#define LIFCR_CTEIF3	(1U << 25)
#define LIFCR_CHTIF3	(1U << 26)
#define LIFCR_CTCIF3	(1U << 27)

void spi1_dma_init(void)
{
	/******************    GPIO configuration    ********************/
	// Enable clock access to SPI1 pins on PortA [PA5, PA6, PA7]
	RCC->AHB1ENR |= GPIOAEN;

	// Set SPI pins mode to AF mode - 0b10
	//PA5
	GPIOA->MODER |= (1U << 11);
	GPIOA->MODER &= ~(1U << 10);

	//PA6
	GPIOA->MODER |= (1U << 13);
	GPIOA->MODER &= ~(1U << 12);

	//PA7
	GPIOA->MODER |= (1U << 15);
	GPIOA->MODER &= ~(1U << 14);

	// Set alternate type to AF5 (SPI1) - 0b0101
	//PA5
	GPIOA->AFR[0] &= ~(1U << 23);
	GPIOA->AFR[0] |= (1U << 22);
	GPIOA->AFR[0] &= ~(1U << 21);
	GPIOA->AFR[0] |= (1U << 20);

	//PA6
	GPIOA->AFR[0] &= ~(1U << 27);SPI1->CR1 |= CR1_MSTR;
	GPIOA->AFR[0] |= (1U << 26);
	GPIOA->AFR[0] &= ~(1U << 25);
	GPIOA->AFR[0] |= (1U << 24);

	//PA7
	GPIOA->AFR[0] &= ~(1U << 31);
	GPIOA->AFR[0] |= (1U << 30);
	GPIOA->AFR[0] &= ~(1U << 29);
	GPIOA->AFR[0] |= (1U << 28);


	/******************   SPI configuration    ********************/
	// Enable clock access to SPI module on APB2
	RCC->APB2ENR |= SPI1EN;

	// Set software slave management
	SPI1->CR1 |= (CR1_SSM | CR1_SSI);

	// Set SPI to master mode
	SPI1->CR1 |= CR1_MSTR;

	// Set CPOL =1 and CPHA = 1
	SPI1->CR1 |= CR1_CPOL;
	SPI1->CR1 |= CR1_CPHA;

	// Set clock divider - bits 5:3 - to 0b001 or PCLK/4
	SPI1->CR1 |= (1U << 3);
	SPI1->CR1 &= ~(1U << 4);
	SPI1->CR1 &= ~(1U << 5);


	// Select to use DMA for both Tx and Rx of SPI data
	SPI1->CR2 |= CR2_TXDMAEN;
	SPI1->CR2 |= CR2_RXDMAEN;

	// Enable SPI
	SPI1->CR1 |= CR1_SPE;
}

void dma2_stream3_spi_tx_init(void)
{
	/*****************  DMA configuration *********************/
	// Enable clock access to DMA on AHB1 bus
	RCC->AHB1ENR |= DMA2EN;

	// Disable DMA stream - not needed I believe (see next command)
	DMA2_Stream3->CR &= ~DMA_SCR_EN;

	// Wait until DMA stream is disabled
	while(DMA2_Stream3->CR & DMA_SCR_EN){}

	/*****************  Configure DMA Stream parameters *********************/
	// Enable memory address increment
	DMA2_Stream3->CR |= DMA_SCR_MINC;

	// Set transfer direction. Here its memory to SPI peripheral for Tx
	DMA2_Stream3->CR |= (1U << 6);
	DMA2_Stream3->CR &= ~(1U << 7);

	// Enable transfer complete interrupt
	DMA2_Stream3->CR |= DMA_SCR_TCIE;

	// Enable transfer error interrupt
	DMA2_Stream3->CR |= DMA_SCR_TEIE;

	// Disable direct mode
	DMA2_Stream3->FCR |= DMA_SFCR_DMDIS;

	// Set DMA FIFO threshold - Full
	DMA2_Stream3->FCR |= (1U << 1);
	DMA2_Stream3->FCR |= (1U << 0);

	// Enable DMA interrupt in NVIC
	NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

void dma2_stream2_spi_rx_init(void)
{
	/*****************  DMA configuration *********************/
	// Enable clock access to DMA on AHB1 bus
	RCC->AHB1ENR |= DMA2EN;

	// Disable DMA stream - not needed I believe (see next command)
	DMA2_Stream2->CR &= ~DMA_SCR_EN;

	// Wait until DMA stream is disabled
	while(DMA2_Stream2->CR & DMA_SCR_EN){}


	/*****************  Configure DMA Stream parameters *********************/
	// Enable memory address increment
	DMA2_Stream2->CR |= DMA_SCR_MINC;

	// Set transfer direction. Here its SPI peripheral to memory for Rx
	DMA2_Stream2->CR &= ~(1U << 6);
	DMA2_Stream2->CR &= ~(1U << 7);

	// Enable transfer complete interrupt
	DMA2_Stream2->CR |= DMA_SCR_TCIE;

	// Enable transfer error interrupt
	DMA2_Stream2->CR |= DMA_SCR_TEIE;

	// Disable direct mode
	DMA2_Stream2->FCR |= DMA_SFCR_DMDIS;

	// Set DMA FIFO threshold - Full
	DMA2_Stream2->FCR |= (1U << 1);
	DMA2_Stream2->FCR |= (1U << 0);

	// Enable DMA interrupt in NVIC
	NVIC_EnableIRQ(DMA2_Stream2_IRQn);
}

void dma2_stream3_spi_Tx(uint32_t msg_to_snd, uint32_t msg_len)
{
	// Clear interrupt flags for stream 3
	DMA2->LIFCR |= LIFCR_CDMEIF3;
	DMA2->LIFCR |= LIFCR_CTEIF3;
	DMA2->LIFCR |= LIFCR_CHTIF3;
	DMA2->LIFCR |= LIFCR_CTCIF3;

	// Set peripheral address
	DMA2_Stream3->PAR = (uint32_t)(&(SPI1->DR));

	// Set memory address
	DMA2_Stream3->M0AR = msg_to_snd;

	// Set transfer length
	DMA2_Stream3->NDTR = msg_len;

	// Enable the DMA stream
	DMA2_Stream3->CR |= DMA_SCR_EN;

}


uint32_t dma2_stream2_spi_Rx(uint32_t received_msg, uint32_t msg_len)
{
	// Clear interrupt flags for stream 3
	DMA2->LIFCR |= LIFCR_CDMEIF2;
	DMA2->LIFCR |= LIFCR_CTEIF2;
	DMA2->LIFCR |= LIFCR_CHTIF2;
	DMA2->LIFCR |= LIFCR_CTCIF2;

	// Set peripheral address
	DMA2_Stream2->PAR = (uint32_t)(&(SPI1->DR));

	// Set memory address
	DMA2_Stream2->M0AR = received_msg;

	// Set transfer length
	DMA2_Stream2->NDTR = msg_len;

	// Enable the DMA stream
	DMA2_Stream2->CR |= DMA_SCR_EN;

	return 0;
}






