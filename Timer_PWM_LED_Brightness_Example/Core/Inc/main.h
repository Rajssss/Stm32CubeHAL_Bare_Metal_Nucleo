 /*
 * main.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>

#define SYS_CLOCK_FREQ_50_MHZ 50
#define SYS_CLOCK_FREQ_84_MHZ 84
#define SYS_CLOCK_FREQ_120_MHZ 120

//Prototypes
void Error_Handler(void);
void System_Clock_Config_HSE(uint8_t clock_freq);
void GIOP_LED_Init(void);
void TIMER2_Init(void);
void UART2_Init(void);


#endif /* INC_MAIN_H_ */
