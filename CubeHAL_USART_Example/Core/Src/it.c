/*
 * it.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 */


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
