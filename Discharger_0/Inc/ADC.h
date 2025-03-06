/*
 * ADC.h
 *
 *  Created on: Jul 24, 2024
 *      Author: beast
 */

#ifndef ADC_H_
#define ADC_H_



#include <stddef.h>
#include <stdlib.h>

#include "main.h"
#include "stm32f303xe.h"


#define ADC1_N_BURST_CONST	10	// Num samples each ADC1 CH1 burst sequence


extern uint32_t ADC1_CH1_sample_freq;
extern uint32_t ADC1_CH1_burst_freq;

extern uint32_t ADC1_IRQ_MASK;


/***************************************************************************************************************************************/

typedef struct ADC1_OBJ {		// Create struct type to store ADC1 CH1 vars

	uint8_t ADC1_CUR_CH;							// Current CH selected for sampling burst

	uint16_t ADC1_CH1_DATA[ADC1_N_BURST_CONST];		// CH1 data buffer
	uint16_t ADC1_CH2_DATA[ADC1_N_BURST_CONST];		// CH2 data buffer
	uint16_t ADC1_CH4_DATA[ADC1_N_BURST_CONST];		// CH4 data buffer

	float ADC1_AIN;									// Voltage value result of burst avg calc

} ADC1_OBJ_ALIAS;

extern ADC1_OBJ_ALIAS ADC1_CH1;			// Define struct obj of type ADC1_OBJ_ALIAS
extern ADC1_OBJ_ALIAS *ADC1_HANDLE;	// Define struct obj pointer to type ADC1_OBJ_ALIAS

extern uint8_t ADC1_OBJ_init(void *pMemory, ADC1_OBJ_ALIAS obj);

/***************************************************************************************************************************************/


extern void ADC1_IRQ_EN(void);
extern void ADC1_IRQ_DIS(void);

extern void ADC1_Clr_Pend_Irq(void);

extern int EnableADC1(void);
extern int DisableADC1(void);

extern int ADC1_IRQ_Config(void);

extern int ADC1_Param_Setup(void);

extern void ADC1_Set_Seq(uint8_t ch);

extern int ADC1_Cal(int mode); //proto

extern int ADC1_Cycle_Start(void);
extern void ADC1_Cycle_Stop(void);

extern uint8_t ADC1_Start_Conv(void);

extern int GetVDDA(void);

extern void ADC1_Process_Data(void);





















#endif /* ADC_H_ */
