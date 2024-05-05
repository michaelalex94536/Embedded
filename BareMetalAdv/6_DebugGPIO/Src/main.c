/* For the 446RE Nucleo  */

/* We need to enable the user pushbutton connected to pin PC13
 *
 * When the push button is not depressed, the pin it is connected
 * to, Pin 13 on GPIO Port C, is high - or logic 1.
 *
 * */

#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"

#define 	GPIOCEN 	(1U << 2)
#define 	PIN13 		(1U << 13)
#define 	BTN_PIN		PIN13

uint8_t button_state;


int main(void)
{
	// Enable clock access to GPIOC
	RCC->AHB1ENR |= GPIOCEN;

	// Set PC13 as input pin
	GPIOC->MODER &= ~(1U << 26);
	GPIOC->MODER &= ~(1U << 27);



	while(1)
	{
		// Check if BTN is pressed - means it will go low, since it is normally HIGH
		// Push button is active low.

			if (GPIOC->IDR & BTN_PIN) // If this is 1, button is not pushed.
			{
				button_state = 0;
			}
			else
			{
				button_state = 1;
			}

			// Add a short delay
			for(int i = 0; i < 90000; i++){}

	}
}
