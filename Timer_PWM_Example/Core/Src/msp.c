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


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_UART2;

	memset(&GPIO_UART2, 0, sizeof(GPIO_UART2));

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



void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_TIM2_PWM;

	//memset(&GPIO_TIM2_PWM, 0, sizeof(GPIO_TIM2_PWM));

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*
	 * PB8  -> TIM2_CH1
	 * PB9  -> TIM2_CH2
	 * PB10 -> TIM2_CH3
	 * PB2 -> TIM2_CH4
	 *
	 */

	GPIO_TIM2_PWM.Alternate = GPIO_AF1_TIM2;
	GPIO_TIM2_PWM.Mode = GPIO_MODE_AF_PP;
	GPIO_TIM2_PWM.Pull = GPIO_NOPULL;
	GPIO_TIM2_PWM.Speed = GPIO_SPEED_FAST;
	GPIO_TIM2_PWM.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &GPIO_TIM2_PWM);

	//NVIC Settings
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
}


