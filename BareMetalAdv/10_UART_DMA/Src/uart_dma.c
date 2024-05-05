/*
 * uart_dma.c
 *
 *  Created on: May 3, 2024
 *      Author: michael
 */

#include "uart_dma.h"

void uart2_rx_tx_init(void)
{
	/*********************  Configure UART GPIO pin ********************/
	// 1) Enable clock access to GPIOA
	// 2) Set PA2 to alternate function (AF) mode
	// 3) Set PA3 to alternate function (AF) mode
	// 4) Set PA2 alternate function type to AF7 (USART2_TX)
	// 5) Set PA3 alternate function type to AF7 (USART2_RX)


	/*******************  Configure UART module  *********************/
	// 6) Enable clock access to UART2
	// 7) Set baudrate
	// 8) Specify using DMA
	// 9) Set transfer direction
	//10) Clear TC (transfer complete) flag
	//11) Enable TCIE (transfer complete interrupt)
	//12) Enable UART module
}

void dma1_init(void)
{

}












































