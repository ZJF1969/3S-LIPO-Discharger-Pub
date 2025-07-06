/*
 * main.h
 *
 *  Created on: Jul 20, 2024
 *      Author: beast
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>

#include "stm32f303xe.h"


#define BOOL unsigned char
#define TRUE 1
#define FALSE 0


extern const uint32_t SYS_clk;


/***************************************************************************************************************************************/

typedef struct Process_Vars_Obj {		// Create struct type to store main process vars

	BOOL SYS_ON;
	BOOL BATT_GOOD;
	BOOL DISCHARGE_ON;

	BOOL ADC_CAPTURES_RUNNING;			// Capture bursts running flag

	float V_C1;							// Cell 1 (TOP) voltage
	float V_C2;							// Cell 2 (MID) voltage
	float V_C3;							// Cell 3 (BOT) voltage

	float Current_Setpoint;				// Either coarse or fine setpoint used in logic


} Process_Vars_Obj_Alias;

extern Process_Vars_Obj_Alias Process_Vars;			// Define struct obj of type Process_Vars_Obj_Alias
extern Process_Vars_Obj_Alias *Process_Vars_Handle;	// Define struct obj pointer to type Process_Vars_Obj_Alias

extern uint8_t Process_Vars_Obj_init(void *pMemory, Process_Vars_Obj_Alias obj);

/***************************************************************************************************************************************/

extern BOOL Debounce(uint8_t input, int *cnt, int *btn_lock);

extern void Millisec(float microSec);

/***************************************************************************************************************************************/




#endif /* MAIN_H_ */
