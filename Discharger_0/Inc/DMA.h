/*
 * DMA.h
 *
 *  Created on: Sep 26, 2024
 *      Author: beast
 */

#ifndef DMA_H_
#define DMA_H_


#include <stdint.h>

#include "stm32f303xe.h"

#include "ADC.h"



extern void DMA_CH1_config(void);

void DMA_CH1_set_write_addr(uint32_t *addr);

























#endif /* DMA_H_ */
