/*
 * main.h
 *
 *  Created on: Sep 28, 2020
 *      Author: Rajssss@GitHub.com
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"


void USART2_Init(void);
void RTCInit(void);
void RTC_Calender_Init(void);
void Error_handler(void);
void UART_Send_Calender(void);

#endif /* INC_MAIN_H_ */
