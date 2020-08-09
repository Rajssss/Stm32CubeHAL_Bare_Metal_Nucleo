/*
 * it.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Rajssss@GitHub.com
 */

#include "main.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
