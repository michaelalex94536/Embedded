/*
 * DMA.c
 *
 *  Created on: Apr 27, 2024
 *      Author: michael
 */

#include "dma.h"

#define DMA2EN			(1U << 22)
#define DMA_SCR_EN		(1U << 0)
#define DMA_SCR_MINC	(1U << 10)
#define DMA_SCR_PINC	(1U << 9)
#define DMA_SCR_TCIE	(1U << 4)
#define DMA_SCR_TEIE	(1U << 2)
#define DMA_SFCR_DMDIS	(1U << 2)



void dma2_mem2mem_config(void)
{
	// Enable clock access to DMA module
	RCC->AHB1ENR |= DMA2EN;

	// Disable DMA stream
	DMA2_Stream0->CR = 0;  // All bits set to 0

	// Wait until stream is disabled
	while((DMA2_Stream0->CR & DMA_SCR_EN)){}
	DMA2_Stream0->CR |= DMA_SCR_TCIE;

	// Configure DMA parameters - memory size, etc.
	// Set MSIZE (memory data size) to half word (16 bits)
	DMA2_Stream0->CR |= (1U << 13);
	DMA2_Stream0->CR &= ~(1U << 14);

	// Set PSIZE (peripheral data size) to half word (16 bits)
	DMA2_Stream0->CR |= (1U << 11);
	DMA2_Stream0->CR &= ~(1U << 12);

	// Enable memory address increment
	DMA2_Stream0->CR |= DMA_SCR_MINC;

	// Enable peripheral address increment
	DMA2_Stream0->CR |= DMA_SCR_PINC;

	// Select memory to memory transfer
	DMA2_Stream0->CR |= (1U << 7);
	DMA2_Stream0->CR &= ~(1U << 6);

	// Enable transfer complete interrupt
	DMA2_Stream0->CR |= DMA_SCR_TCIE;

	// Enable transfer error interrupt
	DMA2_Stream0->CR |= DMA_SCR_TEIE;


	// Disable direct mode
	DMA2_Stream0->FCR |= DMA_SFCR_DMDIS;

	// Set DMA FIFO threshold
	DMA2_Stream0->FCR |= (1U << 1);
	DMA2_Stream0->FCR |= (1U << 0);

	// Enable DMA interrupt in NVIC
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void dma_transfer_start(uint32_t src_buf, uint32_t dest_buf, uint32_t len)
{
	// Set peripheral (source) address
	DMA2_Stream0->PAR = src_buf;

	// Set memory address
	DMA2_Stream0->M0AR = dest_buf;

	// Set transfer length
	DMA2_Stream0->NDTR = len;

	// Enable DMA stream
	DMA2_Stream0->CR |= DMA_SCR_EN;
}








