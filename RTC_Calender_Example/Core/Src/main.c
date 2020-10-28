/*
 * main.c
 *
 *  Created on: Sep 28, 2020
 *      Author: Rajssss@GitHub.com
 *      Description: This Example demonstrates how to configure RTC to setup Date and Time,
 *      			 and read it. The data and time will be sent via UART.
 */

#include "main.h"


UART_HandleTypeDef UART2Handler;
RTC_HandleTypeDef RTC_Calender_Handler;


int main(void)
{
	HAL_Init();

	USART2_Init();

	RTCInit();

	RTC_Calender_Init();

	while(1)
	{
		UART_Send_Calender();
	}


	return 0;
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


void Error_handler(void)
{
	while(1);
}


void RTCInit(void)
{
	RTC_Calender_Handler.Instance = RTC;
	RTC_Calender_Handler.Init.HourFormat = RTC_HOURFORMAT_12;
	RTC_Calender_Handler.Init.AsynchPrediv = 0x7F;
	RTC_Calender_Handler.Init.SynchPrediv = 0xFF;
	RTC_Calender_Handler.Init.OutPut = DISABLE;
	RTC_Calender_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;					//doesn't matter as O/P Disabled
	RTC_Calender_Handler.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;					//same :)

	if(HAL_RTC_Init(&RTC_Calender_Handler) != HAL_OK)
	{
		Error_handler();
	}

}


void RTC_Calender_Init(void)
{
	RTC_DateTypeDef RTC_Date_Init = {0};
	RTC_TimeTypeDef RTC_Time_Init = {0};

	//Configure the Date and Time as 28 September 2020 Monday : 2:49:10 PM

	RTC_Date_Init.Date = 28;
	RTC_Date_Init.Month = RTC_MONTH_SEPTEMBER;
	RTC_Date_Init.Year = 20;															//2020, 2000 is assumed by default
	RTC_Date_Init.WeekDay = RTC_WEEKDAY_MONDAY;
	if(HAL_RTC_SetDate(&RTC_Calender_Handler, &RTC_Date_Init, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	RTC_Time_Init.TimeFormat = RTC_HOURFORMAT12_PM;
	RTC_Time_Init.Hours = 2;
	RTC_Time_Init.Minutes = 49;
	RTC_Time_Init.Seconds = 10;
	if(HAL_RTC_SetTime(&RTC_Calender_Handler, &RTC_Time_Init, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

}



void UART_Send_Calender(void)
{
	static RTC_DateTypeDef hDateRead;
	static RTC_TimeTypeDef hTimeRead;

	char buff[100] = {0};

	HAL_RTC_GetDate(&RTC_Calender_Handler, &hDateRead, RTC_FORMAT_BIN);					//Read Date in hDateRead structure
	HAL_RTC_GetTime(&RTC_Calender_Handler, &hTimeRead, RTC_FORMAT_BIN);					//Read Date in hTimeRead structure

	sprintf(buff, "Date- %02d-%02d-%02d Time- %02d:%02d:%02d\n\r",
				hDateRead.Date, hDateRead.Month, hDateRead.Year, hTimeRead.Hours, hTimeRead.Minutes, hTimeRead.Seconds);

	HAL_UART_Transmit(&UART2Handler, (uint8_t *)buff, sizeof(buff), HAL_MAX_DELAY);
}



