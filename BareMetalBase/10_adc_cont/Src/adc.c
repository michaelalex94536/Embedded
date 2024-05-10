/*
 * adc.c
 *
 * For the F411, ADC1 is on APB2. To enable ADC1, set bit 8 of
 * the RCC_APB2ENR register
 *
 * And ADC1 Channel 1 is connected to Pin A1 of GPIOA (Table 8 on page 51 of the datasheet)
 *
 *  Created on: May 9, 2024
 *      Author: michael
 */

#include "adc.h"

#define ADC1EN			(1U << 8)
#define GPIOAEN			(1U << 0)
#define ADC_CH1			(1U << 0)
#define ADC_SEQ_LEN		0x00
#define CR2_ADON		(1U << 0)
#define CR2_SWSTART		(1U << 30)
#define SR_EOC			(1U << 1)

#define CR2_CONT		(1U << 1)



void adc1_pa1_init(void)
{
	/************    Configure the ADC GPIOA pin    ***************/
	// Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// Set the mode of PA1 to analog mode - bits 3:2 -> 0b11
	GPIOA->MODER |= (1U << 3);
	GPIOA->MODER |= (1U << 2);


	/************    Configure the ADC peripheral    ***************/
	// Enable clock access to ADC
	RCC->APB2ENR |= ADC1EN;

	/************    Configure the ADC parameters    **************/
	// Configure conversion sequence start. Bits 4:0 of ADC_SQR3 -> 0b00001 (ADC channel 1)
	ADC1->SQR3 &= ~(1U << 4);
	ADC1->SQR3 &= ~(1U << 3);
	ADC1->SQR3 &= ~(1U << 2);
	ADC1->SQR3 &= ~(1U << 1);
	ADC1->SQR3 |= ADC_CH1;

	// Conversion sequence length - Bits 23:20 of ADC_SQR1 -> 0b0000
	// For 1 conversion, setting all bits of this register to 0 is OK
	ADC1->SQR1 = ADC_SEQ_LEN;

	// Enable ADC module
	ADC1->CR2 |= CR2_ADON;
}

void start_conversion(void)
{
	// Enable continuous conversion
	ADC1->CR2 |= CR2_CONT;

	// Start ADC conversion
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void)
{
	// Wait for conversion to be complete - check end-of-conversion flag
	while( !(ADC1->SR & SR_EOC)){ } // Sit here when statement is false

	// Read converted result
	return ADC1->DR;
}



