/*
 * main.h
 *
 *  Created on: Jul 20, 2024
 *      Author: beast
 */

#ifndef MAIN_H_
#define MAIN_H_


#include "stm32f303xe.h"
#include "core_cm4.h"
#include "cmsis_gcc.h"

#include <stdlib.h>
#include <stdint.h>

#include "stm32f303re_sys_init.h"
#include "IRQ.h"
#include "PWM.h"
#include "ADC.h"
#include "DMA.h"


/***************************************************************************************************************************************/

typedef struct Process_Vars_Obj {		// Create struct type to store main process vars

	uint8_t ADC1_Idle;
	uint8_t ADC1_CH1_Data_Good;
	uint8_t ADC_Burst_Running;

	float V_C1;					// Cell 1 (TOP) voltage
	float V_C2;					// Cell 2 (MID) voltage
	float V_C3;					// Cell 3 (BOT) voltage


} Process_Vars_Obj_Alias;

extern Process_Vars_Obj_Alias Process_Vars;			// Define struct obj of type Process_Vars_Obj_Alias
extern Process_Vars_Obj_Alias *Process_Vars_Handle;	// Define struct obj pointer to type Process_Vars_Obj_Alias

extern uint8_t Process_Vars_Obj_init(void *pMemory, Process_Vars_Obj_Alias obj);

/***************************************************************************************************************************************/

extern int Debounce(uint16_t input, int *cnt, int *btn_lock);

extern void Millisec(float microSec);

/***************************************************************************************************************************************/




#endif /* MAIN_H_ */
