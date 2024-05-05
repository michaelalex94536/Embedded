/* Uses STM32 F446RE Nucleo board  */

#include "stm32f4xx.h"
#include <string.h>


char _a[1] = {'A'};
char *pt_a = _a;

char hello[] = {"Hello from main"};
char *p_hello = hello;

int main(void)
{

	for(int i = 0; i < strlen(hello); i++)
	{
		ITM_SendChar(*p_hello++);
	}


	while(1)
	{

	}

}
