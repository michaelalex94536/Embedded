/*
 * uart.c
 *
 *  Created on: Apr 26, 2024
 *      Author: michael
 */

#include "uart.h"

// To enable assorted clocks
#define UART2EN		(1U << 17) // UART2 clock enable
#define GPIOAEN		(1U << 0)  // GPIOA clock enable

// To put UART into Tx (TE) or Rx (RE) mode, and enable it (UE)
#define CR1_TE		(1U << 3)
#define CR1_RE		(1U << 2)
#define CR1_UE		(1U << 13)
#define SR_TXE		(1U << 7) //Transmit data register is empty

// Needed to set the USART baudrate
#define UART_BAUDRATE	115200
#define CLK				16000000  // Default clock for this chip

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart2_write(int ch);
int __io_putchar(int ch);


void uart2_tx_init(void)
{
	// Configure GPIO pins as UART2 pins (Alternate Function mode):
	// 1) Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// 2) Set PA2 mode to alternate function (AF) mode
	GPIOA->MODER &= ~(1U << 4);  // Set bit 4 to 0
	GPIOA->MODER |= (1U << 5);   // Set bit 5 to 1

	// 3) Set PA2 AF function to USART2 Tx, which is AF7int __io_putchar(int ch)

	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] |= (1U << 9);
	GPIOA->AFR[0] |= (1U << 10);
	GPIOA->AFR[0] &= ~(1U << 11);

	// Configure UART module:
	// 4) Enable clock access to UART2
	RCC->APB1ENR |= UART2EN;
	// 5) Set baudrate
	uart_set_baudrate(CLK, UART_BAUDRATE);

	// 6) Set transfer direction AND CLEAR ALL OTHER BITS IN THIS REGISTER!
	USART2->CR1 = CR1_TE;

	// 7) Enable UART module, not changing any other bits (as usual)
	USART2->CR1 |= CR1_UE;

}

// Function to write data byte to USART2
static void uart2_write(int ch)
{
	/* Make sure the transmit data register is empty  */
	while(!(USART2->SR & SR_TXE)){}

	/* Write to transmit data register.  */
	USART2->DR = (ch & 0xFF);

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

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

