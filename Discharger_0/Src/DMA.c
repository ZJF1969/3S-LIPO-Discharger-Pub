/*
 * DMA.c
 *
 *  Created on: Sep 26, 2024
 *      Author: beast
 */

#include "DMA.h"


void DMA_CH1_config(void) {

	DMA1_Channel1->CCR &= ~(0x1 << 4);	// Select read from periph mode
	DMA1_Channel1->CCR |= (0x1 << 5);	// EN DMA1 CH1 cir mode
	DMA1_Channel1->CCR |= (0x1 << 7);	// EN DMA1 CH1 mem increment mode
	DMA1_Channel1->CCR |= (0x1 << 8);	// Select transfer size from periph, 16 bits
	DMA1_Channel1->CCR |= (0x1 << 10);	// Select transfer size to mem, 16 bits
	DMA1_Channel1->CCR |= (0x3 << 12);	// Select very high priority

	DMA1_Channel1->CNDTR = ADC1_N_BURST_CONST;					// Set for 10 transfers, = to ADC1 CH1 burst size

	DMA1_Channel1->CPAR = &ADC1->DR;							// Set periph address, ADC1 data reg

	//DMA1_Channel1->CMAR = &ADC1_HANDLE->ADC1_CH1_DATA[0];			// Set mem address, ADC1 CH1 data array

	DMA1_Channel1->CCR |= 0x1;									// EN DMA1

}


void DMA_CH1_set_write_addr(uint32_t *addr){

	DMA1_Channel1->CCR &= ~0x1;		// Disable DMA1 CH1 to be able to write to mem addr

	DMA1_Channel1->CMAR = (uint32_t)addr;	// Set mem addr

	DMA1_Channel1->CCR |= 0x1;		// EN DMA CH1 again


}

















