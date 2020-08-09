/*
 * main.h
 *
 *  Created on: Aug 8, 2020
 *      Author: Rajssss@GitHub.com
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"


//Prototypes
void SystemClockConfig(void);
void USART2_Init(void);
void USART2_SendData(void);
void Error_handler(void);
void SYSTICK_ReConfig(void);


#endif /* INC_MAIN_H_ */
