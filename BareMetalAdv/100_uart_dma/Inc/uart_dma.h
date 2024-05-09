/*
 * uart_dma.h
 *
 *  Created on: May 3, 2024
 *      Author: michael
 */

#ifndef UART_DMA_H_
#define UART_DMA_H_

#include <stdint.h>
#include "stm32f4xx.h"
#define UART_DATA_BUF_SIZE		5

void uart2_rx_tx_init(void);
void dma1_init(void);
void dma1_stream5_uart_rx_config(void);
void dma1_stream6_uart_tx_config(uint32_t msg_to_send, uint32_t msg_len);


#endif /* UART_DMA_H_ */
