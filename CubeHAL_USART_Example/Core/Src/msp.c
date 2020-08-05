/*
 * msp.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 */

#include "stm32f4xx_hal.h"


/*
 * Low Level Initializations
 * */
void HAL_MspInit()
{
	//Setup Priority Grouping
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//Enable Required exceptions (here ex: USGFAULT, BUSFAULT, MEMFAULT)
	SCB->SHCSR |= (0x7 << 16);

	//Configure Priority of the exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

}
