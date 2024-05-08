/*
 * adc_dma.c
 *
 *  Created on: Apr 30, 2024
 *      Author: michael
 */

/* ADC1 is on the APB2 bus  */
/* GPIOA is on the AHB1 bus  */

/* Must use DMA2 Stream0 for ADC1 as seen in the reference manual */

#include "adc_dma.h"


#define GPIOAEN			(1U << 0)
#define ADC1EN			(1U << 8)
#define CR1_SCAN		(1U << 8)

#define CR2_DMA			(1U << 8)
#define CR2_DDS			(1U << 9)
#define CR2_CONT		(1U << 1)
#define CR2_ADCON		(1U << 0)
#define CR2_SWSTART		(1U << 30)

#define DMA2EN			(1U << 22)
#define DMA_SCR_EN		(1U << 0)
#define DMA_SCR_MINC	(1U << 10)
#define DMA_SCR_PINC	(1U << 9)
#define DMA_SCR_CIRC	(1U << 8)
#define DMA_SCR_TCIE	(1U << 4)
#define DMA_SCR_TEIE	(1U << 2)
#define DMA_SFCR_DMDIS	(1U << 2)

#define TIM2EN			(1U << 0)
#define CR1_CEN			(1U << 0)

uint16_t adc_raw_data[NUM_SAMPLES];

void adc_tim_dma_init(void)
{
	/*****************  GPIO configuration *********************/
	// Enable clock access to ADC GPIO pins on port A
	RCC->AHB1ENR |= GPIOAEN;

	// Set pin PA0 to analog mode:
	// PA0
	GPIOA->MODER |= (1U << 0);
	GPIOA->MODER |= (1U << 1);


	/*****************  ADC configuration *********************/
	// Enable clock access to ADC1 on the APB2 bus
	RCC->APB2ENR |= ADC1EN;

	// Select to use DMA, but now use external trigger
	ADC1->CR2 |= (CR2_DMA | CR2_DDS);

	// Here select external trigger on rising edge
	ADC1->CR2 |= (1U << 28);
	ADC1->CR2 &= ~(1U << 29);

	// External trigger event is Timer 2 TRGO [Bits 27:24 = 0b0110]
	ADC1->CR2 &= ~(1U << 27);
	ADC1->CR2 |= (1U << 26);
	ADC1->CR2 |= (1U << 25);
	ADC1->CR2 &= ~(1U << 24);

	// Set ADC resolution using ADC_CR1 register bits 25:24.
	// 0b00 = 12-bit resolution, 0b10 = 8-bit resolution.
	ADC1->CR1 |= (1U << 25);
	ADC1->CR1 &= ~(1U << 24);


	/*****************  DMA configuration *********************/
	// Enable clock access to DMA on AHB1 bus
	RCC->AHB1ENR |= DMA2EN;

	// Disable DMA stream - not needed I believe (see next command)
	DMA2_Stream0->CR &= ~DMA_SCR_EN;

	// Wait until DMA is disabled
	while(DMA2_Stream0->CR & DMA_SCR_EN){}

	// Enable circular mode
	DMA2_Stream0->CR |= DMA_SCR_CIRC;

	// Set memory transfer size to half word
	DMA2_Stream0->CR |= (1U << 13);
	DMA2_Stream0->CR &= ~(1U << 14);

	// Set peripheral transfer size to half word
	DMA2_Stream0->CR |= (1U << 11);
	DMA2_Stream0->CR &= ~(1U << 12);

	// Enable memory address increment
	DMA2_Stream0->CR |= DMA_SCR_MINC;

	// Enable transfer complete interrupt
	DMA2_Stream0->CR |= DMA_SCR_TCIE;

	// Set peripheral address - ADC register
	DMA2_Stream0->PAR = (uint32_t)(&(ADC1->DR));

	// Set memory address
	DMA2_Stream0->M0AR = (uint32_t)(&adc_raw_data);

	// Set number of transfers
	DMA2_Stream0->NDTR = (uint16_t)NUM_SAMPLES;

	// Enable DMA interrupt in NVIC
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);


	/*****************  Timer configuration *********************/

	/* Configure timer period to be 100 Hz: ADC sampling rate is 100 Hz */

	// Enable clock access to Tim2 on the APB1 bus
	RCC->APB1ENR |= TIM2EN;

	// Set TIM2 pre-scaler value
	TIM2->PSC = 16000 - 1; 			//   16MHz / 16000 = 1kHz

	// Set TIM2 auto-reload value
	TIM2->ARR = 10 - 1;				//	  1kHz / 10 = 100Hz


	// Configure master mode selection bits 6:4 to 0b010  [Update mode]
	TIM2->CR2 &= ~(1U << 4);
	TIM2->CR2 |= (1U << 5);
	TIM2->CR2 &= ~(1U << 6);



	/*****************  Enable modules *********************/
	// Turn on ADC1
	ADC1->CR2 |= CR2_ADCON;

	// Enable DMA stream
	DMA2_Stream0->CR |= DMA_SCR_EN;

	// Enable Timer
	TIM2->CR1 |= CR1_CEN;

}


