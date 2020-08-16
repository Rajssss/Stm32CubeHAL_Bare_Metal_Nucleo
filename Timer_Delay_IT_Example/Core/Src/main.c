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

	GIOP_LED_Init();

	TIM6_Init();

	if(HAL_TIM_Base_Start_IT(&TIM6_Handler) != HAL_OK)
	{
		Error_Handler();
	}

	while(1);

	return 0;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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


void GIOP_LED_Init(void)
{
	//Initialize GPIOA5 (User LED)
	GPIO_InitTypeDef GPIOLed;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIOLed.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOLed.Speed = GPIO_SPEED_FAST;
	GPIOLed.Pull = GPIO_NOPULL;
	GPIOLed.Pin = GPIO_PIN_5;

	HAL_GPIO_Init(GPIOA, &GPIOLed);
}
