/*
 * main.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 *      Description: This Example uses a basic timer (TIM6) to generate interrupt for every 100ms
 *      			 to toggle the user LED.
 *
 */

#include "main.h"


TIM_HandleTypeDef TIM6_Handler;


int main(void)
{
	HAL_Init();

	System_Clock_Config();

	TIM6_Init();

	while(1);

	return 0;
}

void System_Clock_Config()
{
	//Not Required
}

void Error_Handler(void)
{
	while(1);

}



void TIM6_Init(void)
{
	TIM6_Handler.Instance = TIM6;
	TIM6_Handler.Init.Prescaler = 24;
	TIM6_Handler.Init.Period = 64000 - 1;
	if(HAL_TIM_Base_Init(&TIM6_Handler) != HAL_OK)
	{
		Error_Handler();
	}
}
