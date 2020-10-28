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


void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef TIM2_IC_GPIO;

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	TIM2_IC_GPIO.Alternate = GPIO_AF1_TIM2;
	TIM2_IC_GPIO.Mode = GPIO_MODE_AF_PP;
	TIM2_IC_GPIO.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &TIM2_IC_GPIO);

	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_UART2;

	GPIO_UART2.Alternate = GPIO_AF7_USART2;
	GPIO_UART2.Mode = GPIO_MODE_AF_PP;
	GPIO_UART2.Pin = GPIO_PIN_2;		//USART2 Tx
	GPIO_UART2.Pull = GPIO_PULLUP;
	GPIO_UART2.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &GPIO_UART2);

	GPIO_UART2.Pin = GPIO_PIN_3;		//USART2 Rx
	HAL_GPIO_Init(GPIOA, &GPIO_UART2);

	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);


}
