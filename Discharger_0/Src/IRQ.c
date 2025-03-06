/*
 * IRQ.c
 *
 *  Created on: Jul 6, 2024
 *      Author: beast
 */


#include "IRQ.h"


/***************************************************************************************************************************************/

void DisableIRQ(void){__disable_irq();}

/***************************************************************************************************************************************/

void EnableIRQ(void){__enable_irq();}

/***************************************************************************************************************************************/

uint8_t hb_counter = 0;
uint8_t hb_flag = 0;

void SysTick_Handler(void){

	hb_counter++;
	Heartbeat();

}

/***************************************************************************************************************************************/

void Heartbeat(void){

	if (hb_counter == 200){

		if (hb_flag == 1){

			GPIOA -> BSRR |= (1 << 5);
			hb_flag--;
			hb_counter = 0;
		}
		else if (hb_flag == 0) {

			GPIOA -> BSRR |= (1 << 21);
			hb_flag++;
			hb_counter = 0;
		}
	}
}

/***************************************************************************************************************************************/

/* Timer for PWM*/


void TIM2_IRQHandler(void){


}

/***************************************************************************************************************************************/

/* Timer for Millisec function */

int ms_done_flag = 0;

void TIM3_IRQHandler(void){

	TIM3 -> SR &= 0; //clear interrupt flag

	ms_done_flag = 1; // Set done flag

}

/***************************************************************************************************************************************/

/*	Timer for ADC1 bursts	*/

void TIM4_IRQHandler(void){

	TIM4->SR &= ~0x1;																				// Clr IRQ flag

	if (Process_Vars_Handle->ADC_Burst_Running == 0x0 && Process_Vars_Handle->ADC1_Idle == 0x1){	// If burst not running and ADC1 idle

		ADC1_HANDLE->ADC1_NEXT_CH = 0x1;															// Reset to first ADC1 CH
		Process_Vars_Handle->ADC_Burst_Running = 0x1;												// Start ADC1 burst
	}

}

/***************************************************************************************************************************************/

/*	ADC1 IRQ	*/

void ADC1_2_IRQHandler(void){

	switch (ADC1 -> ISR & ADC1_IRQ_MASK) {		// Mask ISR reg with expected IRQ flags

		case ADC_ISR_EOS:						// EOS flag

			ADC1->ISR |= 0x1;								// Clr IRQ flags
			ADC1_Clr_Pend_Irq();							// Clr pending IRQ

			Process_Vars_Handle->ADC1_Idle = 0x1;			// ADC1 conv done

			break;


		case ADC_ISR_OVR:						// OVR flag

			ADC1->CR |= (0x1 << 4);							// Stop ADC current conv
			ADC1->ISR |= 0x1;								// Clr IRQ flags
			ADC1_Clr_Pend_Irq();							// Clr pending IRQ

			Process_Vars_Handle->ADC1_CH1_Data_Good = 0x0;	// ADC1 bad data
			Process_Vars_Handle->ADC1_Idle = 0x1;			// ADC1 done

			break;


		default:

			ADC1->ISR |= 0x1;								// Clr IRQ flags
			ADC1_Clr_Pend_Irq();							// Clr pending IRQ

			Process_Vars_Handle->ADC1_CH1_Data_Good = 0x0;	// ADC1 bad data
			Process_Vars_Handle->ADC1_Idle = 0x1;			// ADC1 done

	}

}

/***************************************************************************************************************************************/




