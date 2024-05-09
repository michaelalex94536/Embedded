/*
 * dma.h
 *
 *  Created on: Apr 27, 2024
 *      Author: michael
 */

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define LISR_TCIF0		(1U << 5)
#define LIFCR_CTCIF0	(1U << 5)

#define LISR_TEIF0		(1U << 3)
#define LIFCR_CTEIF0	(1U << 3)

void dma2_mem2mem_config(void);
void dma_transfer_start(uint32_t src_buf, uint32_t dest_buf, uint32_t len);

#endif /* DMA_H_ */
