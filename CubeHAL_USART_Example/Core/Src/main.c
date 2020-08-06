/*
 * main.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 */

#include "main.h"
#include "stm32f4xx_hal.h"


UART_HandleTypeDef USART2_Handler;


int main()
{
	HAL_Init();

	SystemClockConfig();

	USART2_Init();

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


