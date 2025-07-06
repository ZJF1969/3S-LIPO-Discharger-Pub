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
#include "DMA.h"


#define ADC1_N_BURST_CONST	10	// Num samples each ADC1 CH1 burst sequence

extern const uint8_t ADC1_TOTAL_CAPTURES;

extern uint32_t ADC1_CH1_sample_freq;
extern const uint32_t ADC1_CH1_BURST_FREQ;

extern uint32_t ADC1_IRQ_MASK;


/***************************************************************************************************************************************/

typedef struct ADC1_OBJ {		// Create struct type to store ADC1 CH1 vars

	BOOL ADC1_IDLE;						// ADC1 idle, not converting flag
	BOOL ADC1_DATA_GOOD;				// ADC1 error flag for inter capture routine catching

	uint8_t ADC1_CURRENT_CAPTURE;					// Iterate var to track which input to capture

	uint16_t ADC1_CH1_DATA[ADC1_N_BURST_CONST];		// CH1 data buffer
	uint16_t ADC1_CH2_DATA[ADC1_N_BURST_CONST];		// CH2 data buffer
	uint16_t ADC1_CH6_DATA[ADC1_N_BURST_CONST];		// CH4 data buffer

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

extern int ADC1_Cal(int mode);

extern int ADC1_Cycle_Start(void);
extern void ADC1_Cycle_Stop(void);

extern uint8_t ADC1_Start_Conv(uint8_t ch, uint16_t *buffer);

extern int GetVDDA(void);

extern float ADC1_Process_Data(uint16_t buffer[]);






















#endif /* ADC_H_ */
