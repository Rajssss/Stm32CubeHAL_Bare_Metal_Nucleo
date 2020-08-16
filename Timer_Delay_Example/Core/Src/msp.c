/*
 * msp.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 */

#include "main.h"


void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	SCB->SHCSR |= (0x7 << 16);

	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//enable clock for TIM6
	__HAL_RCC_TIM6_CLK_ENABLE();

	//Enable TIM6 IRQ
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//setup priority
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
}



