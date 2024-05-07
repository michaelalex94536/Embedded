#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

#include "uart.h"

// We're going to use the user LED
#define GPIOAEN			(1U << 0)
#define PIN5			(1U << 5)
#define LED_PIN			PIN5

char key;

int main(void)
{
	// Enable GPIOA clock
	RCC->AHB1ENR |= GPIOAEN;

	// Set Pin 5 on GPIOA as an output
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);


	uart2_rxtx_init();

	while(1)
	{
		key = uart2_read();

		if(key == '1')
		{
			GPIOA->ODR |= LED_PIN;

			printf("Got a '1' from the keyboard...\n\r");
		}
		else
		{
			GPIOA->ODR &= ~LED_PIN;
		}
	}
}
