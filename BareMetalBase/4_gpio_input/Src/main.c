/*
 * The user pushbutton is connected to PC13 - Port C, pin 13
 *
 * The button pin is pulled high, which means it is "active low."
 */

#include "stm32f4xx.h"

#define GPIOAEN			(1U << 0)
#define GPIOCEN			(1U << 2)
#define PIN5			(1U << 5)
#define LED_PIN			PIN5
#define PIN13			(1U << 13)
#define BTN_PIN			PIN13

int main(void)
{

	// Enable GPIOA and GPIOC clocks
	RCC->AHB1ENR |= (GPIOCEN | GPIOAEN);

	// Set Pin 5 on GPIOA as an output
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	// Set Pin 13 on GPIOC as an input - which is the reset configuration...
	GPIOC->MODER &= ~(1U << 26);
	GPIOC->MODER &= ~(1U << 27);

	while(1)
	{
		// Check if button is pressed
		if(GPIOC->IDR & BTN_PIN)		// If this is true, the button HAS NOT been pressed
		{
			GPIOA->BSRR = (1U << 21);   	// Write to the bit reset register and turn off LED
		}
		else // The button pin has gone low - the button has been pressed
		{
			GPIOA->BSRR = LED_PIN;  // Write to the bit set register and turn on LED
		}

	}
}
