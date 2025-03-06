/*
 * PWM.c
 *
 *  Created on: Jul 10, 2024
 *      Author: beast
 */

#include "PWM.h"


float PWM_freq = 1000000;
float SYS_clk = 72000000;

float SYS_clk_adj = 0.0139;	// Amount of SYS_clk to subtract from TIM period calc, hard coded


/***************************************************************************************************************************************/

/*	Using CH2 for CC PWM output, mapped to PA1 GPIO pin	*/

void SetupPWM(void){

	TIM2->ARR = round((SYS_clk * (1 - SYS_clk_adj)) / PWM_freq);	// Load TIM2
	TIM2->CCR2 = (int)((float)TIM2->ARR * 0.3);						// Preload the CC reg for D = 30%

	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;									// EN compare reg preload
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;				// Setup for PWM mode 1

	TIM2 -> CR1 |= 0x1; 											//start timer

}

/***************************************************************************************************************************************/

void StartPWM(void){

	TIM2->CCER |= TIM_CCER_CC2E;	// CC CH2 EN to start PWM

}

/***************************************************************************************************************************************/

void StopPWM(void){

	TIM2->CCER &= ~TIM_CCER_CC2E;	// CC CH2 DIS to stop PWM

}

/***************************************************************************************************************************************/

void UpdatePWM(float d){

	TIM2->CCR2 = (int)((float)TIM2->ARR * d);	// Update the CC preload reg for new D

}

/***************************************************************************************************************************************/



