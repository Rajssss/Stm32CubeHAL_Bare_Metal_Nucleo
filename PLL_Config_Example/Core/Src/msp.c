/*
 * msp.c
 *
 *  Created on: Aug 8, 2020
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


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef UART2_GPTIO_Handler;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	UART2_GPTIO_Handler.Alternate = GPIO_AF7_USART2;
	UART2_GPTIO_Handler.Mode = GPIO_MODE_AF_PP;
	UART2_GPTIO_Handler.Pull = GPIO_PULLUP;
	UART2_GPTIO_Handler.Speed = GPIO_SPEED_FAST;
	UART2_GPTIO_Handler.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOA, &UART2_GPTIO_Handler);

	UART2_GPTIO_Handler.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &UART2_GPTIO_Handler);

}



