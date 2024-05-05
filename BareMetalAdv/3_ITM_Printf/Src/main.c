/* Uses STM32 F446RE Nucleo board  */

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>


int __io_putchar(int ch)
{
	ITM_SendChar(ch);

	return ch;
}



int main(void)
{

	printf("Printf is now available.\n");

	while(1)
	{

	}

}
