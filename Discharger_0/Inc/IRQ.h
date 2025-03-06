/*
 * IRQ.h
 *
 *  Created on: Jul 6, 2024
 *      Author: beast
 */

#ifndef IRQ_H_
#define IRQ_H_


#include "main.h"
#include "PWM.h"
#include "ADC.h"

#include "cmsis_gcc.h"
#include "stm32f303xe.h"

#include <stdint.h>


extern int ms_done_flag;


extern void DisableIRQ(void);
extern void EnableIRQ(void);

extern void SysTick_Handler(void);

extern void Heartbeat(void);

extern void TIM2_IRQHandler(void);

extern void TIM3_IRQHandler(void);

extern void delay(int time);






#endif /* IRQ_H_ */
