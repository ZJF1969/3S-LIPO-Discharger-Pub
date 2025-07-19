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


/***************************************************************************************************************************************/

/* Defines */

#define ADC_CONVS_IN_BURST	10			// Num samples each ADC burst sequence


/***************************************************************************************************************************************/

/* Externed Vars */

extern const uint8_t ADC1_TOTAL_CAPTURES;

extern uint32_t ADC1_CH1_sample_freq;
extern const uint32_t ADC1_CH1_BURST_FREQ;

extern uint32_t ADC1_IRQ_MASK;


/***************************************************************************************************************************************/

typedef struct ADC1_OBJ {		// Create struct type to store ADC1 CH1 vars

	BOOL ADC1_IDLE;						// ADC1 idle, not converting flag
	BOOL ADC1_DATA_GOOD;				// ADC1 error flag for inter capture routine catching

	uint8_t ADC1_CURRENT_CAPTURE;					// Iterate var to track which input to capture

	uint16_t ADC_CH0_DATA[ADC_CONVS_IN_BURST];		// CH0 data buffer
	uint16_t ADC_CH1_DATA[ADC_CONVS_IN_BURST];		// CH1 data buffer
	uint16_t ADC_CH2_DATA[ADC_CONVS_IN_BURST];		// CH2 data buffer
	uint16_t ADC_CH3_DATA[ADC_CONVS_IN_BURST];		// CH3 data buffer
	uint16_t ADC_CH4_DATA[ADC_CONVS_IN_BURST];		// CH4 data buffer

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
