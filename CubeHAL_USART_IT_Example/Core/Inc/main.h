/*
 * main.h
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define TRUE	1
#define FALSE	0

//Prototypes
void SystemClockConfig(void);
void USART2_Init(void);
void USART2_Error_Handler(void);

uint8_t Convert_To_Upper(uint8_t data);


#endif /* INC_MAIN_H_ */
