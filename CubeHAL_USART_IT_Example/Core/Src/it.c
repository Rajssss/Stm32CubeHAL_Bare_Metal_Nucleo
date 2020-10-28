/*
 * it.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 */

#include "main.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


extern UART_HandleTypeDef USART2_Handler;


//USART2 IRQ Handler
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&USART2_Handler);
}
