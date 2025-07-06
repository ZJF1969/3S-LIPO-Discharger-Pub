/*
 * stm32f303re_sys_init.h
 *
 *  Created on: Jul 6, 2024
 *      Author: beast
 */

#ifndef STM32F303RE_SYS_INIT_H_
#define STM32F303RE_SYS_INIT_H_


#include "ADC.h"
#include "main.h"

#include "math.h"



extern void SysInit(void);

extern void GPIOConfig(void);

extern BOOL BattCheck(void);

extern void TIMConfig(void);

extern void start_cell_check_timer(void);

extern int ADC_INIT(void);

extern void DMA1_init(void);

extern void OPAMP_init(void);



























#endif /* STM32F303RE_SYS_INIT_H_ */
