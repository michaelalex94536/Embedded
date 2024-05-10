/*
ADC - continuous conversion for one channel only on ADC1 channel 1 - Pin PA1
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

	// Since ADC is in continuous conversion mode, this doesn't need to be in while loop
	start_conversion();

	while(1)
	{

		sensor_value = adc_read();

		printf("Sensor value: %d\n\r", (int)sensor_value);

	}
}
