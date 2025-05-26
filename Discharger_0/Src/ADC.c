/*
 * ADC.c
 *
 *  Created on: Jul 24, 2024
 *      Author: beast
 */

#include "ADC.h"


#define ADC1_Res	4095	// Configure ADC1 for 12-bit resolution

uint32_t ADC1_CH1_burst_freq = 5;


uint32_t ADC1_IRQ_MASK = 0;

ADC1_OBJ_ALIAS ADC1_CH1;			// Define struct obj of type ADC1_OBJ_ALIAS
ADC1_OBJ_ALIAS *ADC1_HANDLE;	// Define struct obj pointer to type ADC1_OBJ_ALIAS


float VDDA_Meas = 3.3;


/***************************************************************************************************************************************/

/*	Function to init the ADC1_CH1 struct obj, assign handle the obj address if successful	*/

uint8_t ADC1_OBJ_init(void *pMemory, ADC1_OBJ_ALIAS obj){

	ADC1_HANDLE = (ADC1_OBJ_ALIAS*)calloc(1, sizeof(obj));

	if (ADC1_HANDLE == NULL) { return 1; }

    ADC1_HANDLE = (ADC1_OBJ_ALIAS*)pMemory;

    return 0;

}
/***************************************************************************************************************************************/

void ADC1_IRQ_EN(void){ NVIC_EnableIRQ(ADC1_2_IRQn); }
void ADC1_IRQ_DIS(void){ NVIC_DisableIRQ(ADC1_2_IRQn); }

void ADC1_Clr_Pend_Irq(void){ NVIC_ClearPendingIRQ(ADC1_2_IRQn); }

/***************************************************************************************************************************************/

int EnableADC1(void){

	if ( (	(!((ADC1 -> CR >> 1) & 0x1)) &				// Check for no ADC disable in progress
			((ADC1 -> CR >> 28) & 0x3) &				// Check for Vreg EN
			(!((ADC1 -> CR >> 31) & 0x1))) == 1) {		// Check for no Cal in progress

		ADC1 -> CR |= 0x1;

		while (!(ADC1 -> ISR & 0x1));	// Wait for ADC1 ready flag

		return 0;

	}
	else { return 1; }	// Return error, ADC1 cannot EN

}

/***************************************************************************************************************************************/

int DisableADC1(void){

	if ( (	((ADC1 -> CR & 0x1)) &						// Check for ADC1 EN
			(!((ADC1 -> CR >> 2) & 0x1))) == 1) {		// Check for no Conver in progress

		ADC1 -> CR |= (0x1 << 1);	// DIS ADC1

		ADC1 -> ISR |= 0x1;			// Clear ADC1 rdy flag

		return 0;

	}
	else { return 1; }

}

/***************************************************************************************************************************************/

int ADC1_IRQ_Config(void){

	if (~((ADC1 -> CR >> 2) & 0x1)) {		// Check for no Conver in progress

		NVIC_SetPriority(ADC1_2_IRQn, 0x1);

		ADC1->IER |= (0x1 << 3);			// EN EOS IRQ
		ADC1->IER |= (0x1 << 4);			// EN OVR IRQ

		ADC1_IRQ_MASK |= ADC_ISR_EOS | ADC_ISR_OVR;	// Set the mask with expected IRQs

		return 0;
	}
	else { return 1; }

}

/***************************************************************************************************************************************/

int ADC1_Param_Setup(void){

	if (!((ADC1 -> CR >> 2) & 0x1)) {		// Check for no Conver in progress

		ADC1 -> SMPR1 |= (0x4 << 3);		// Set CH1 sample time to 19.5 clk cycles, 2.25MHz Fs

		ADC1->CFGR |= 0x1;					// EN DMA for ADC1
		ADC1->CFGR |= (0x1 << 1);			// EN DMA circular mode


		return 0;

	}
	else { return 1; }

}

/***************************************************************************************************************************************/

/*	Config sequence reg for a burst of 10 conv on ch arg  */
/*	No conv check, access only through ADC1_Start_Conv	*/

void ADC1_Set_Seq(uint8_t ch){

		ADC1->SQR1 = 0x0;					// Clr ADC1 sequencing regs
		ADC1->SQR2 = 0x0;
		ADC1->SQR3 = 0x0;

		ADC1->SQR1 |= 0x9;					// Set ADC1 for 10 conv burst, N+1

		ADC1->SQR1 |= (ch << 6) | (ch << 12) | (ch << 18) | (ch << 24);			// Set ch arg as pos 1-10
		ADC1->SQR2 |= ch | (ch << 6) | (ch << 12) | (ch << 18) | (ch << 24);
		ADC1->SQR3 |= ch;

}

/***************************************************************************************************************************************/

int ADC1_Cal(int mode){

	if (((!(ADC1 -> CR & 0x1)) &					// Check for no ADC1 EN
		(!(ADC1 -> CR & (1 << 2))) &				// Check for no Conver in progress
		(!(ADC1 -> CR & (1 << 31)))) == 1) {		// Check for no Cal in progress

		ADC1 -> CR |= (mode << 30);		// Set to single ended or diff mode
		ADC1 -> CR |= (1 << 31);		// Start ADC1 Cal

		while (ADC1 -> CR & (1 << 31));		// Wait for Cal completion

		return 0;

	}

	else { return 1; }	// Return error, ADC1 cannot Cal

}

/***************************************************************************************************************************************/

/*	Start TIM4 to run burst of conv every 200mS	  */

int ADC1_Cycle_Start(void){

	if ( (	((ADC1 -> CR & 0x1)) &						// Check for ADC1 EN
			(~((ADC1 -> CR >> 1) & 0x1))) == 1) {		// Check for no DIS request

		NVIC_EnableIRQ(TIM4_IRQn);					// Enable TIM4 IRQ to start ADC1 cycle

		return 0;

	}
	else { return 1; }

}

/***************************************************************************************************************************************/

void ADC1_Cycle_Stop(void){

	NVIC_DisableIRQ(TIM4_IRQn);		// Dis TIM4 IRQ to stop ADC1 cycle

}

/***************************************************************************************************************************************/

/*	Start an ADC1 conversion							*/
/*	Args: Channel to run, addr of destination buffer	*/

uint8_t ADC1_Start_Conv(uint8_t ch, uint16_t *buffer){

	if (!((ADC1 -> CR >> 2) & 0x1)) {		// Check for no Conver in progress

		ADC1_Set_Seq(ch);					// Update current ADC1 CH

		DMA_CH1_set_write_addr(buffer);		// Write next destination address

		ADC1 -> CR |= (0x1 << 2);			// Start ADC1 conv

		return 0;

	}
	else { return 1; }

}

/***************************************************************************************************************************************/

int GetVDDA(void){

	//uint8_t state = 0;

	if (!(ADC1 -> CR & (1 << 2))) {						// Check for no Conver in progress

		float VDDA_Nom = 3.3;

	    uint16_t *data_ptr = (uint16_t *) 0x1FFFF7BA;	// Get Vrefint data at mem adr

		ADC1 -> SMPR2 |= (0x7 << 24);					// Set ADC1 CH18 to min 2.2uS sample time for Vrefint conver

		ADC1->SQR1 = 0x0;								// Clr sequence reg
		ADC1->SQR1 |= (0x12 << 6);						// Set CH18 to first conv pos to read Vref

		ADC1 -> CR |= (0x1 << 2);						// Start ADC1 conv

		while (!(ADC1 -> ISR & (0x1 << 2)));			// Wait for ADC1 EOC flag assert

		VDDA_Meas = VDDA_Nom * ((float)*data_ptr / (float)ADC1 -> DR);	// Calc VDDA from ref manual

		ADC1->ISR |= 0x1;			// Clr IRQ flags
		ADC1_Clr_Pend_Irq();		// Clr pending IRQ

		return 0;

	}

	else { return 1; }

}

/***************************************************************************************************************************************

void ADC1_Process_Data(void){

	float val = 0;
	float accum = 0;

	for (uint8_t i = 0; i < ADC1_N_BURST_CONST; i++){

		val = VDDA_Meas / ADC1_Res * ADC1_HANDLE->ADC1_DATA[i];

		accum = accum + val;

	}

	ADC1_HANDLE->ADC1_AIN = accum / ADC1_N_BURST_CONST;

}

***************************************************************************************************************************************/



















