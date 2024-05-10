/*
ADC - single convesion for one channel only on ADC1 channel 1 - Pin PA1
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{

	uart2_tx_init();

	adc1_pa1_init();


	while(1)
	{

		start_conversion();

		sensor_value = adc_read();

		printf("Sensor value: %d\n\r", (int)sensor_value);

	}
}
