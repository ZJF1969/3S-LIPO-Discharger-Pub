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

	BOOL PROCESS_RUNNING;				// Process running
	BOOL BATT_GOOD;						// Battery connected
	BOOL DISCHARGE_ON;					// Battery is discharging

	BOOL ADC_CAPTURES_RUNNING;			// Capture bursts running flag

	float V_C1;							// Cell 1 (TOP) voltage
	float V_C2;							// Cell 2 (MID) voltage
	float V_C3;							// Cell 3 (BOT) voltage

	float Current_Setpoint;				// Either coarse or fine setpoint used in logic


} Process_Vars_Obj_Alias;

extern Process_Vars_Obj_Alias Process_Vars;			// Define struct obj of type Process_Vars_Obj_Alias
extern Process_Vars_Obj_Alias *PROCESS_VARS_HANDLE;	// Define struct obj pointer to type Process_Vars_Obj_Alias

extern uint8_t Process_Vars_Obj_init(void *pMemory, Process_Vars_Obj_Alias obj);

/***************************************************************************************************************************************/

/* Mapped ADC channels */

enum ADC_Channels {

	CH0,	// 0,	Cell 1
	CH1,	// 1, 	Cell 2
	CH2,	// 2, 	Cell 3
	CH3,	// 3, 	Batt Current
	CH4		// 4, 	NTC

};


/***************************************************************************************************************************************/

extern void error_catch(void);

extern BOOL Debounce(uint8_t input, int *cnt, int *btn_lock);

extern void Millisec(float microSec);

/***************************************************************************************************************************************/




#endif /* MAIN_H_ */
