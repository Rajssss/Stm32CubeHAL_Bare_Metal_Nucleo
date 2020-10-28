/*
 * main.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 *      Description: This Example uses a general purpose timer (TIM2) as Output Capture and
 *      			 Generates Square waves of frequencies 500Hz, 1KHz, 2KHz, 4KHz via channels 1,2,3 and 4 resp.
 *
 */

#include "main.h"

//PULSE Values for different signal generation
#define PULSE_VAL_500HZ		25000
#define PULSE_VAL_1KHZ		12500
#define PULSE_VAL_2KHZ		6250
#define PULSE_VAL_4KHZ		3125

TIM_HandleTypeDef TIM2_Handler;
UART_HandleTypeDef UART2Init;


int main(void)
{
	HAL_Init();

	System_Clock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);		//Increases Accuracy

	GIOP_LED_Init();

	UART2_Init();

	TIMER2_Init();

	if(HAL_TIM_OC_Start_IT(&TIM2_Handler, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&TIM2_Handler, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&TIM2_Handler, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_TIM_OC_Start_IT(&TIM2_Handler, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}


	while(1)
	{
		;
	}

	return 0;
}



void System_Clock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	uint8_t flash_latency = 0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
	Osc_Init.HSIState = RCC_HSI_ON;			//not needed as on by default
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.HSICalibrationValue = 16;		//default
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 50;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;

		  flash_latency = 1;
	     break;

	  case SYS_CLOCK_FREQ_84_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 84;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;

		  flash_latency = 2;
	     break;

	  case SYS_CLOCK_FREQ_120_MHZ:
		  Osc_Init.PLL.PLLM = 4;
		  Osc_Init.PLL.PLLN = 120;
		  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
		  Osc_Init.PLL.PLLQ = 2;
		  Osc_Init.PLL.PLLR = 2;
		  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
		  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
		  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;

		  flash_latency = 3;
	     break;

	  default:
	   return ;
	 }

	if(HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
	{
			Error_Handler();
	}

	if(HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
	{
		Error_Handler();
	}


	/*Configure the systick timer interrupt frequency (for every 1 ms) */
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}



void Error_Handler(void)
{
	while(1);

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


void TIMER2_Init(void)
{
	TIM_OC_InitTypeDef TIM2_OC_Init;

	TIM2_Handler.Instance = TIM2;
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM2_Handler.Init.Prescaler = 1;		//Divide clock by 2
	TIM2_Handler.Init.Period = 0xFFFFFFFF;
	if(HAL_TIM_OC_Init(&TIM2_Handler) != HAL_OK)
	{
		Error_Handler();
	}

	TIM2_OC_Init.OCMode = TIM_OCMODE_TOGGLE;
	TIM2_OC_Init.OCPolarity = TIM_OCPOLARITY_HIGH;

	//Channel-1 -> 500Hz
	TIM2_OC_Init.Pulse = PULSE_VAL_500HZ;
	if(HAL_TIM_OC_ConfigChannel(&TIM2_Handler, &TIM2_OC_Init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	//Channel-2 -> 1KHz
	TIM2_OC_Init.Pulse = PULSE_VAL_1KHZ;
	if(HAL_TIM_OC_ConfigChannel(&TIM2_Handler, &TIM2_OC_Init, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	//Channel-3 -> 2KHz
	TIM2_OC_Init.Pulse = PULSE_VAL_2KHZ;
	if(HAL_TIM_OC_ConfigChannel(&TIM2_Handler, &TIM2_OC_Init, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}

	//Channel-4 -> 4KHz
	TIM2_OC_Init.Pulse = PULSE_VAL_4KHZ;
	if(HAL_TIM_OC_ConfigChannel(&TIM2_Handler, &TIM2_OC_Init, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}

}


void UART2_Init(void)
{
	UART2Init.Instance = USART2;
	UART2Init.Init.BaudRate = 115200;
	UART2Init.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART2Init.Init.Mode = UART_MODE_TX_RX;
	UART2Init.Init.Parity = UART_PARITY_NONE;
	UART2Init.Init.StopBits = UART_STOPBITS_1;
	UART2Init.Init.WordLength = UART_WORDLENGTH_8B;
	if(HAL_UART_Init(&UART2Init) != HAL_OK)
	{
		Error_Handler();
	}
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + PULSE_VAL_500HZ));
	}
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) + PULSE_VAL_1KHZ));

	}
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3) + PULSE_VAL_2KHZ));

	}
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, (HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4) + PULSE_VAL_4KHZ));

	}


}


