/*
 * main.c
 *
 *  Created on: 08-Aug-2020
 *      Author: Rajssss@GitHub.com
 *      Description: This Program Configures SYSCLK and Other Bus Clocks
 *      			 as per the following and Send the configured frequencies
 *      			 via USART2(Async mode).
 *      		  ->PLL as SYSCLK with I/P clock HSI
 *      		  ->User selectable frequencies of 50, 84, 120 and 180MHz.
 *      		  ->Make sure the respective us frequencies do you exceed max limit.
 */

#include "main.h"
#include <string.h>
#include <stdio.h>


#define SYSCLK_FREQ_50MHZ	50
#define SYSCLK_FREQ_84MHZ	84
#define SYSCLK_FREQ_120MHZ	120
#define SYSCLK_FREQ_180MHZ	180



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
	SystemClockConfig(SYSCLK_FREQ_180MHZ);

	//AS HCLK is changed, Reconfigure the SYSTICK.
	SYSTICK_ReConfig();

	//Reinitialize USART2 as Clock(APB1) frequency changed.
	//(Baud rate depends on Peripheral Bus clock frequency.)
	USART2_Init();

	//Send the configured clock values
	USART2_SendData();

	return 0;

}



void SystemClockConfig(uint8_t CLK_Freq)
{
	uint8_t FlashLatency = 0;

	//In our case, not required as HSI is already default clock source (Just for demonstration)
	OSC_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	OSC_Init.HSIState = RCC_HSI_ON;
	OSC_Init.HSICalibrationValue = 16;  //Default

	//Configure PLL as required
	OSC_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	OSC_Init.PLL.PLLState = RCC_PLL_ON;

	//Set SYSCLK as per CLK_Freq
	switch(CLK_Freq)
	{
		case SYSCLK_FREQ_50MHZ:
		{
			//configure various PLL prescalars
			OSC_Init.PLL.PLLM = 16;
			OSC_Init.PLL.PLLN = 100;
			OSC_Init.PLL.PLLP = 2;
			OSC_Init.PLL.PLLQ = 2;
			OSC_Init.PLL.PLLR = 2;

			//Configure General prescalars
			CLK_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			CLK_Init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
			CLK_Init.AHBCLKDivider = RCC_HCLK_DIV1;
			CLK_Init.APB1CLKDivider = RCC_HCLK_DIV2;
			CLK_Init.APB2CLKDivider = RCC_HCLK_DIV1;

			FlashLatency = FLASH_LATENCY_1;

			break;
		}

		case SYSCLK_FREQ_84MHZ:
		{
			//configure various PLL prescalars
			OSC_Init.PLL.PLLM = 8;
			OSC_Init.PLL.PLLN = 84;
			OSC_Init.PLL.PLLP = 2;
			OSC_Init.PLL.PLLQ = 2;
			OSC_Init.PLL.PLLR = 2;

			//Configure General prescalars
			CLK_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			CLK_Init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
			CLK_Init.AHBCLKDivider = RCC_HCLK_DIV1;
			CLK_Init.APB1CLKDivider = RCC_HCLK_DIV2;
			CLK_Init.APB2CLKDivider = RCC_HCLK_DIV1;

			FlashLatency = FLASH_LATENCY_2;

			break;
		}

		case SYSCLK_FREQ_120MHZ:
		{
			//configure various PLL prescalars
			OSC_Init.PLL.PLLM = 8;
			OSC_Init.PLL.PLLN = 120;
			OSC_Init.PLL.PLLP = 2;
			OSC_Init.PLL.PLLQ = 2;
			OSC_Init.PLL.PLLR = 2;

			//Configure General prescalars
			CLK_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			CLK_Init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
			CLK_Init.AHBCLKDivider = RCC_HCLK_DIV1;
			CLK_Init.APB1CLKDivider = RCC_HCLK_DIV4;
			CLK_Init.APB2CLKDivider = RCC_HCLK_DIV2;

			FlashLatency = FLASH_LATENCY_3;

			break;
		}

		case SYSCLK_FREQ_180MHZ:
		{
			//To reach 180MHz we have to configure Voltage Regulation Power Scale
			//Enable Voltage Scaling and Over Drive
			__HAL_RCC_PWR_CLK_ENABLE();
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
			__HAL_PWR_OVERDRIVE_ENABLE();

			//configure various PLL prescalars
			OSC_Init.PLL.PLLM = 8;
			OSC_Init.PLL.PLLN = 180;
			OSC_Init.PLL.PLLP = 2;
			OSC_Init.PLL.PLLQ = 2;
			OSC_Init.PLL.PLLR = 2;

			//Configure General prescalars
			CLK_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			CLK_Init.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
			CLK_Init.AHBCLKDivider = RCC_HCLK_DIV1;
			CLK_Init.APB1CLKDivider = RCC_HCLK_DIV4;
			CLK_Init.APB2CLKDivider = RCC_HCLK_DIV2;

			FlashLatency = FLASH_LATENCY_5;

			break;
		}
		default:
		{
			return;
		}
	}

	if(HAL_RCC_OscConfig(&OSC_Init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&CLK_Init, FlashLatency) != HAL_OK)
	{
		Error_handler();
	}

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

