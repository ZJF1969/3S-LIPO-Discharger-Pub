/*
 * stm32f303re_sys_init.c
 *
 *  Created on: Jul 6, 2024
 *      Author: beast
 */



#include "stm32f303re_sys_init.h"

/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field.
                                  This value must be a multiple of 0x200. */

#define ADC1_INPUT_MODE 0x0		// 0x0 -> Single Ended, 0x1 -> Differential
float T_ADC_VREG_STARTUP = 11; 	// 10uS + 1uS for good measure

int systick_ticks = 72000; //1mS

uint32_t sys_clk = 72000000;


/***************************************************************************************************************************************/

void SysInit(void) {

	RCC -> CR |= RCC_CR_HSION;
	while (! ( RCC -> CR & (RCC_CR_HSIRDY)));

	FLASH -> ACR |= FLASH_ACR_LATENCY_2 | FLASH_ACR_PRFTBE;

	RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV2;
	RCC -> CFGR |= RCC_CFGR_PPRE2_DIV1;

	RCC -> CFGR |= RCC_CFGR_PLLSRC_HSI_PREDIV;
	RCC -> CFGR |= RCC_CFGR_PLLMUL9;

	RCC -> CR |= RCC_CR_PLLON;
	while (! (RCC -> CR & RCC_CR_PLLRDY));

	RCC -> CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC -> CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	RCC -> APB1ENR |= RCC_APB1ENR_PWREN;	// Power interface clk EN
	RCC -> APB2ENR |= 0x1;					// SYSCFG clk EN

	FPUInit();

	SysTick_Config(systick_ticks);

}

/***************************************************************************************************************************************/

void FPUInit(void)
{
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  // set CP10 and CP11 Full Access
#endif
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; // Vector Table Relocation in Internal SRAM
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; // Vector Table Relocation in Internal FLASH
#endif
}

/***************************************************************************************************************************************/

void GPIOConfig(void){

	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN; 	// GPIOA clk EN
	RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;	// GPIOB clk EN
	RCC -> AHBENR |= RCC_AHBENR_GPIOCEN;	// GPIOC clk EN

	GPIOA -> MODER |= (1 << 10); 	//configure LED output at PA5


	/*	ADC	 */

	GPIOA->MODER |= 0x3;		 	// Set ADC1_CH1 input PA0 to analog mode
	GPIOA->MODER |= (0x3 << 2);		// Set ADC1_CH2 input PA1 to analog mode
	GPIOA->MODER |= (0x3 << 6);		// Set ADC1_CH4 input PA3 to analog mode


	/*	PWM	 */

	//GPIOA->MODER |= (0x2 << 2);												// Config PA1 as alt func
	//GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2_0 | GPIO_OSPEEDER_OSPEEDR2_1;	// Config PA1 as high speed
	//GPIOA->AFR[0] |= (0x1 << 4);												// Config PA1 alt function to TIM2 CH2 CC


	/*	User Button	 */

	GPIOC -> PUPDR |= (2 << 26); 	//config PC13 pulldown for button

	
	/*	OPAMPs	*/

	//GPIOA->MODER |= (0x3 << 14);	// Set OPAMP1 non-inv input PA7 to analog mode
	//GPIOA->MODER |= (0x3 << 4);		// Set OPAMP1 output PA2 to analog mode

	GPIOB->MODER |= 0x3;			// Set OPAMP2 non-inv input PB0 to analog mode
	GPIOA->MODER |= (0x3 << 12);	// Set OPAMP2 output PA6 to analog mode

	GPIOB->MODER |= (0x3 << 26);	// Set OPAMP3 non-inv input PB13 to analog mode
	GPIOB->MODER |= (0x3 << 2);		// Set OPAMP3 output PB1 to analog mode
	
	GPIOA->MODER |= (0x3 << 8);		// Set OPAMP4 non-inv input PA4 to analog mode
	GPIOB->MODER |= (0x3 << 24);	// Set OPAMP4 output PB12 to analog mode

}

/***************************************************************************************************************************************/

void TIMConfig(void){

	/* TIM2 INIT - PWM */

	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN; 	// Enable TIM2 clock

	TIM2->CR1 |= TIM_CR1_URS;   			// Only overflow gen UEV


	/* TIM3 INIT - Millisec */

	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN; 	// Enable TIM3 clock

	TIM3 -> CR1 |= TIM_CR1_OPM;				// Set TIM2 to one pulse mode
	TIM3 -> CR1 |= TIM_CR1_URS; 			//
	TIM3 -> DIER |= TIM_DIER_UIE;			//

	NVIC_SetPriority(TIM3_IRQn, 0x09);		//Set TIM3 IRQ priority in the NVIC
	NVIC_EnableIRQ(TIM3_IRQn);				// Enable TIM3 IRQ


	/* TIM4 INIT - ADC */

	RCC -> APB1ENR |= RCC_APB1ENR_TIM4EN;				// Enable TIM4 clock

	TIM4->CR1 |= TIM_CR1_URS; 							// Only overflow gen UEV
	TIM4->DIER |= TIM_DIER_UIE;							// EN UEV IRQ
	TIM4->PSC |= 0xFA;									// Set prescaler to 250
	TIM4->ARR = sys_clk / 0xFA / ADC1_CH1_burst_freq;	// Load timer with burst freq value

	NVIC_SetPriority(TIM4_IRQn, 0x4);					//Set TIM4 IRQ priority in the NVIC

	TIM4->CR1 |= 0x1;									// Start timer


}

/***************************************************************************************************************************************/

int ADC_INIT(void){				// PG 380 for adc calcs

	uint8_t state1 = 0;
	uint8_t state2 = 0;

	RCC -> AHBENR |= RCC_AHBENR_ADC12EN;	// enable ADC1 and 2 clock

	ADC12_COMMON -> CCR |= (1 << 16);	// Set ADC1 and ADC 2 clock as the sys clk

	ADC1 -> CR &= (0 << 29);	// first place ADC voltage regulator in intermediate state
	ADC1 -> CR |= (1 << 28);	// enable ADC1 vreg

	Millisec(T_ADC_VREG_STARTUP); // Wait 11uS for vreg startup

	ADC12_COMMON -> CCR |= (1 << 22);	// Enable Vrefint channel

	state1 = ADC1_Cal(ADC1_INPUT_MODE);	// Calibrate ADC1

	state2 = ADC1_OBJ_init(&ADC1_CH1, ADC1_CH1); // Init the ADC1 CH1 obj

	if ((!state1 & !state2) == 1) { return 0; }
	else { return 1; }


}

/***************************************************************************************************************************************/

void DMA1_init(void){

	RCC->AHBENR |= 0x1;			// EN DMA1 clk

}

/***************************************************************************************************************************************/

void OPAMP_init(void){

	//OPAMP1->CSR |= 0x1;		// EN OPAMP1
	OPAMP2->CSR |= 0x1;		// EN OPAMP2
	OPAMP3->CSR |= 0x1;		// EN OPAMP3
	OPAMP4->CSR |= 0x1; 	// EN OPAMP4

	//OPAMP1->CSR &= (0b00 << 2);		// Map OPAMP1 non-inv input to PA7
	//OPAMP1->CSR |= (0x3 << 5); 		// Set OPAMP1 to follower mode, non inv input

	OPAMP2->CSR |= (0x2 << 2);		// Map OPAMP2 non-inv input to PB0
	OPAMP2->CSR |= (0x3 << 5); 		// Set OPAMP2 to follower mode, non inv input

	OPAMP3->CSR |= (0b00 << 2);		// Map OPAMP3 non-inv input to PB13
	OPAMP3->CSR |= (0x3 << 5); 		// Set OPAMP3 to follower mode, non inv input
	
	OPAMP4->CSR |= (0x2 << 2);		// Map OPAMP4 non-inv input to PA4
	OPAMP4->CSR |= (0x3 << 5);		// Set OPAMP4 to follower mode, non inv input

}

/***************************************************************************************************************************************/





