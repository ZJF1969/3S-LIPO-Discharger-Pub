/*
 * PWM.h
 *
 *  Created on: Jul 10, 2024
 *      Author: beast
 */

#ifndef PWM_H_
#define PWM_H_

//#include "main.h"
//#include "stm32f303xe.h"

extern float PWM_freq;
//extern float SYS_clk;

extern float SYS_clk_adj;

extern void SetupPWM(void);

extern void StartPWM(void);
extern void StopPWM(void);

extern void UpdatePWM(float Duty);


#endif /* PWM_H_ */
