/*
 * adc_dma.h
 *
 *  Created on: Apr 30, 2024
 *      Author: michael
 */

#ifndef ADC_DMA_H_
#define ADC_DMA_H_

#include "stm32f4xx.h"
#include <stdint.h>

void adc_tim_dma_init(void);

#define NUM_SAMPLES	10

#define LISR_TCIF0		(1U << 5)
#define LIFCR_CTCIF0	(1U << 5)

#endif /* ADC_DMA_H_ */
