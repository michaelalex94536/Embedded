/* Uses STM32 F446RE Nucleo board  */

#include "stm32f4xx.h"

int sum;

int add(int num1, int num2)
{
	int temp = 0;
	temp = num1 + num2;
	return temp;
}



int main(void)
{
	int total;

	for(int i = 0; i < 10; i++)
	{
		total += i;
	}

	sum = add(10, 20);

	while(1)
	{

	}

}
