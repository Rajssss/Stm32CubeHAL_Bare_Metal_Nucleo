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



//USART Low Level Inits
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_USART2;

	//Enable Peripheral Clocks
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//GPIO Pin Mux config
	GPIO_USART2.Pin = GPIO_PIN_2;
	GPIO_USART2.Alternate = GPIO_AF7_USART2;
	GPIO_USART2.Mode = GPIO_MODE_AF_PP;
	GPIO_USART2.Pull = GPIO_PULLUP;
	GPIO_USART2.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_USART2);

	GPIO_USART2.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &GPIO_USART2);


	//IRQ and Priority Config
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

}
