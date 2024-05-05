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

void adc_dma_init(void);

#define NUM_CHANNELS	2

#endif /* ADC_DMA_H_ */
