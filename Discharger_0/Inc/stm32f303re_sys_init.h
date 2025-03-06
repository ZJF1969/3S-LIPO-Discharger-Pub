/*
 * stm32f303re_sys_init.h
 *
 *  Created on: Jul 6, 2024
 *      Author: beast
 */

#ifndef STM32F303RE_SYS_INIT_H_
#define STM32F303RE_SYS_INIT_H_

#include "stm32f303xe.h"
#include "core_cm4.h"
#include "ADC.h"
#include "main.h"
#include "PWM.h"
#include "math.h"




extern void SysInit(void);

extern void GPIOConfig(void);

extern void TIMConfig(void);

extern int ADC_INIT(void);

extern void DMA1_init(void);

extern void OPAMP_init(void);



























#endif /* STM32F303RE_SYS_INIT_H_ */
