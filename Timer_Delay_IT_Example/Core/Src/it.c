/*
 * it.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 */

#include "main.h"

extern TIM_HandleTypeDef TIM6_Handler;


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}



void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM6_Handler);
}

