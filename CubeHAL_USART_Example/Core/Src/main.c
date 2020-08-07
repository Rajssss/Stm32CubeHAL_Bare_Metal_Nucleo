/*
 * main.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 *      Description: This Program transmits user data via USART2
 *      			 in Async mode.
 */

#include <string.h>
#include "main.h"
#include "stm32f4xx_hal.h"


UART_HandleTypeDef USART2_Handler;


//User Data
char *USER_DATA= "Hello Embedded World!\r\n";



int main()
{
	HAL_Init();

	SystemClockConfig();

	USART2_Init();

	HAL_UART_Transmit(&USART2_Handler, (uint8_t*)USER_DATA, strlen(USER_DATA), HAL_MAX_DELAY);

	while(1);

	return 0;
}



void SystemClockConfig(void)
{
	//No need here, as we will use default clock config.

}


//Initialize USART2 in Async mode
void USART2_Init(void)
{
	USART2_Handler.Instance = USART2;
	USART2_Handler.Init.BaudRate = 115200;
	USART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	USART2_Handler.Init.Mode = UART_MODE_TX_RX;
	USART2_Handler.Init.Parity = UART_PARITY_NONE;
	USART2_Handler.Init.StopBits = UART_STOPBITS_1;
	USART2_Handler.Init.WordLength = UART_WORDLENGTH_8B;

	if(HAL_UART_Init(&USART2_Handler) != HAL_OK)
	{
		//Something wrong
		USART2_Error_Handler();
	}

}


//USART Error handler
void USART2_Error_Handler()
{
	while(1);

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


