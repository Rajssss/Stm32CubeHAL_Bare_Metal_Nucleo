/*
 * main.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 *      Description: This Program transmits user data receives a string via USART2(Async)
 *      			 and convert the lower case letters to upper case then send it.
 *
 */

#include <string.h>
#include "main.h"


UART_HandleTypeDef USART2_Handler;


//User Data
char *USER_DATA= "Hello Embedded World!\r\n";

uint8_t Rcvd_Data;
uint8_t Data_Buffer[100];
uint8_t Rcv_Cmplt = FALSE;		//flag
uint32_t count = 0;


int main()
{
	HAL_Init();

	SystemClockConfig();

	USART2_Init();

	HAL_UART_Transmit(&USART2_Handler, (uint8_t*)USER_DATA, strlen(USER_DATA), HAL_MAX_DELAY);

	while(Rcv_Cmplt != TRUE)
	{
		HAL_UART_Receive_IT(&USART2_Handler, &Rcvd_Data, 1);		//Receive Data Byte by Byte
	}


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



//convert lower case to upper from received data
uint8_t Convert_To_Upper(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		//covert to lower case
		data = data - 32;
	}
	else
	{
		//skip if upper case
	}

	return data;

}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//If Enter key is detected
	if(Rcvd_Data == '\r')
	{
		Rcv_Cmplt = TRUE;
		Data_Buffer[count++] = '\r';
		HAL_UART_Transmit(huart, Data_Buffer, count, HAL_MAX_DELAY);
	}
	else
	{
		Data_Buffer[count++] = Convert_To_Upper(Rcvd_Data);
	}

}




