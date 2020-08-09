/*
 * main.c
 *
 *  Created on: 08-Aug-2020
 *      Author: Rajssss@GitHub.com
 *      Description: This Program Configures SYSCLK and Other Bus Clocks
 *      			 as per the following and Send the configured frequencies
 *      			 via USART2(Async mode).
 *      		  ->HSE as SYSCLK at 8MHz.
 *      		  ->AHB Clock at 4MHz.
 *      		  ->APB1 Clock at 2MHz.
 *      		  ->APB2 Clock at 2MHz.
 *
 */

#include "main.h"
#include <string.h>
#include <stdio.h>


UART_HandleTypeDef UART2Handler;
RCC_OscInitTypeDef OSC_Init;
RCC_ClkInitTypeDef CLK_Init;


char msg[100];


int main(void)
{
	HAL_Init();

	USART2_Init();

	//Send Default Clock Values first
	USART2_SendData();

	//Configure clock as required
	SystemClockConfig();

	//AS HCLK is changed, Reconfigure the SYSTICK.
	SYSTICK_ReConfig();

	//Reinitialize USART2 as Clock(APB1) frequency changed.
	//(Baud rate depends on Peripheral Bus clock frequency.)
	USART2_Init();

	//Send the configured clock values
	USART2_SendData();

	return 0;

}



void SystemClockConfig(void)
{
	OSC_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OSC_Init.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&OSC_Init) != HAL_OK)
	{
		Error_handler();
	}

	CLK_Init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	CLK_Init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	CLK_Init.APB1CLKDivider = RCC_HCLK_DIV2;
	CLK_Init.APB2CLKDivider = RCC_HCLK_DIV2;
	CLK_Init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	if (HAL_RCC_ClockConfig(&CLK_Init, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE();

}



void USART2_Init(void)
{
	UART2Handler.Instance = USART2;
	UART2Handler.Init.BaudRate = 115200;
	UART2Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART2Handler.Init.Mode = UART_MODE_TX;
	UART2Handler.Init.Parity = UART_PARITY_NONE;
	UART2Handler.Init.StopBits = UART_STOPBITS_1;
	UART2Handler.Init.WordLength = UART_WORDLENGTH_8B;
	if( HAL_UART_Init(&UART2Handler) != HAL_OK)
	{
		Error_handler();
	}
}



void USART2_SendData(void)
{
	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ldHz\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&UART2Handler,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK : %ldHz\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&UART2Handler,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK1 : %ldHz\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&UART2Handler,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK2 : %ldHz\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&UART2Handler,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
}



void SYSTICK_ReConfig(void)
{
	//we have changed the clock source and its frequency,
	//so we have to reconfigure the SYSTICK according to that.

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	//SYSCLK prescalar config
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}


void Error_handler(void)
{
	while(1);
}

