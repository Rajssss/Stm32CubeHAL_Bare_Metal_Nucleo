 /*
 * main.h
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"


//Prototypes
void Error_Handler(void);
void System_Clock_Config(void);
void GIOP_LED_Init(void);
void TIM6_Init(void);

#endif /* INC_MAIN_H_ */
