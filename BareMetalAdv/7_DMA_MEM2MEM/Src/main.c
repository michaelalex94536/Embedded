/* For the 446RE Nucleo  */

#include "stm32f4xx.h"
#include <stdio.h>
#include "uart.h"
#include "dma.h"

#define BUFFER_SIZE 	5


uint16_t sensor_data_arr[BUFFER_SIZE] = {892, 731, 1234, 90, 23};	// Source data

uint16_t temp_data_arr[BUFFER_SIZE];								// Destination

volatile uint8_t g_transfer_complete;

int main(void)
{
	g_transfer_complete = 0;

	uart2_tx_init();

	dma2_mem2mem_config();

	dma_transfer_start((uint32_t) sensor_data_arr, (uint32_t) temp_data_arr, BUFFER_SIZE);

	// Wait until transfer is complete:
	while(!g_transfer_complete){}

	for(int i = 0; i < BUFFER_SIZE;i++)
	{
		printf("Temp data buffer[%d]: %d\r\n", i, temp_data_arr[i]);

	}
	g_transfer_complete = 0;

	while(1)
	{


	}
}


void DMA2_Stream0_IRQHandler(void)
{
	// Check if transfer complete interrupt has occurred
	if( (DMA2->LISR) & LISR_TCIF0)
		{
		g_transfer_complete = 1;

		// Clear flag
		DMA2->LIFCR |= LIFCR_CTCIF0;
		}

	// Check if transfer error occurred
	if( (DMA2->LISR) & LISR_TEIF0)
	{
		//Clear flag
		DMA2->LIFCR |= LIFCR_CTEIF0;
	}

}



