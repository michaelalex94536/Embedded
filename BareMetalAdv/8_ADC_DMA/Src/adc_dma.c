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

uint16_t adc_raw_data[NUM_CHANNELS];

void adc_dma_init(void)
{
	/*****************  GPIO configuration *********************/
	// Enable clock access to ADC GPIO pins on port A
	RCC->AHB1ENR |= GPIOAEN;

	// Set pins PA0 and PA1 to analog mode:
	// PA0
	GPIOA->MODER |= (1U << 0);
	GPIOA->MODER |= (1U << 1);

	// PA1
	GPIOA->MODER |= (1U << 2);
	GPIOA->MODER |= (1U << 3);

/*
	//GPIOA->MODER |= (1U << 4);
	//GPIOA->MODER |= (1U << 5);

	// PA3
	GPIOA->MODER |= (1U << 6);
	GPIOA->MODER |= (1U << 7);

	// PA4
	GPIOA->MODER |= (1U << 8);
	GPIOA->MODER |= (1U << 9);
*/
	/*****************  ADC configuration *********************/
	// Enable clock access to ADC1 on the APB2 bus
	RCC->APB2ENR |= ADC1EN;

	// Set sequence length (4 bits for this) to give two (2) conversions: 0b0001
	ADC1->SQR1 |= (1U << 20);
	ADC1->SQR1 &= ~(1U << 21);
	ADC1->SQR1 &= ~(1U << 22);
	ADC1->SQR1 &= ~(1U << 23);

	// Set sequence - OK to clear all other bits.
	ADC1->SQR3 = (0U << 0) | (1U << 5);

	// Enable scan mode (clear all other bits!)
	ADC1->CR1 = CR1_SCAN;

	// Select to use DMA, continuous mode
	ADC1->CR2 |= (CR2_DMA | CR2_CONT | CR2_DDS);




	/*****************  DMA configuration *********************/
	// Enable clock access to DMA on AHB1 bus
	RCC->AHB1ENR |= DMA2EN;

	// Disable DMA stream - not needed I believe (see next command)
	DMA2_Stream0->CR &= ~DMA_SCR_EN;

	// Wait until DMA is disabled
	while(DMA2_Stream0->CR &= ~DMA_SCR_EN){}


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


	// Set peripheral address - ADC register
	DMA2_Stream0->PAR = (uint32_t)(&(ADC1->DR));

	// Set memory address
	DMA2_Stream0->M0AR = (uint32_t)(&adc_raw_data);

	// Set number of transfers
	DMA2_Stream0->NDTR = (uint16_t)NUM_CHANNELS;

	// Enable DMA stream
	DMA2_Stream0->CR |= DMA_SCR_EN;

	/*****************  ADC configuration *********************/
	// Turn on ADC1
	ADC1->CR2 |= CR2_ADCON;

	// Start ADC
	ADC1->CR2 |= CR2_SWSTART;
}
