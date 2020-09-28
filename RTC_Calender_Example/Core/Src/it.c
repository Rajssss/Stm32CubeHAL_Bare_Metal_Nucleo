/*
 * it.c
 *
 *  Created on: Sep 28, 2020
 *      Author: Rajssss@GitHub.com
 */

#include "main.h"


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

