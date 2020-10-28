/*
 * main.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 *      Description: This Example uses a general puspose timer (TIM2) as Input Capture via Channel 1
 *      			 and calculate the frequency of the applied signal, which is 32.768Khz OnBoard LSE.
 *
 */

#include "main.h"


TIM_HandleTypeDef TIM2_Handler;
UART_HandleTypeDef UART2Init;


uint32_t IP_Capture_Count[2];
uint8_t count;
uint8_t is_captured;


int main(void)
{
	double TIM2_CNT_Freq = 0;
	double TIM2_Resolution = 0;
	double IP_Siganl_Freq = 0;
	uint32_t counter_diff = 0;
	char sDataBuffer[50];

	HAL_Init();

	//System_Clock_Config(SYS_CLOCK_FREQ_50_MHZ);

	System_Clock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);		//Increases Accuracy

	GIOP_LED_Init();

	UART2_Init();

	TIMER2_Init();

	LSE_Config();

	HAL_TIM_IC_Start_IT(&TIM2_Handler, TIM_CHANNEL_1);

	while(1)
	{
		if(is_captured == 1)
		{
			if(IP_Capture_Count[1] > IP_Capture_Count[0])
			{
				counter_diff = IP_Capture_Count[1] - IP_Capture_Count[0];
			}
			else
			{
				counter_diff = (0xFFFFFFFF - IP_Capture_Count[0]) + IP_Capture_Count[1];
			}

			TIM2_CNT_Freq = (HAL_RCC_GetPCLK1Freq() * 2) / (TIM2_Handler.Init.Prescaler + 1);
			TIM2_Resolution = 1 / TIM2_CNT_Freq;
			IP_Siganl_Freq = 1 / (counter_diff * TIM2_Resolution);			//(counter_diff * TIM2_Resolution) will give the time period

			sprintf(sDataBuffer, "Freq of I/P Signal => %f\r\n", IP_Siganl_Freq);
			HAL_UART_Transmit(&UART2Init, (uint8_t *)sDataBuffer, strlen(sDataBuffer), HAL_MAX_DELAY);

			is_captured = 0;

		}
	}

	return 0;
}


void System_Clock_Config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	uint8_t flash_latency = 0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
	Osc_Init.HSIState = RCC_HSI_ON;				//not needed on by def
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_Init.HSICalibrationValue = 16;			////not needed / def
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	 {
	  case SYS_CLOCK_FREQ_50_MHZ:
		  Osc_Init.PLL.PLLM = 8;
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
		  Osc_Init.PLL.PLLM = 8;
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
		  Osc_Init.PLL.PLLM = 8;
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


void System_Clock_Config_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	uint8_t flash_latency = 0;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	Osc_Init.HSIState = RCC_HSI_ON;			//not needed as on by default
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
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
	TIM_IC_InitTypeDef TIM2_IC_Init;

	TIM2_Handler.Instance = TIM2;
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM2_Handler.Init.Period = 0xFFFFFFFF;
	TIM2_Handler.Init.Prescaler = 1;
	if(HAL_TIM_IC_Init(&TIM2_Handler) != HAL_OK)
	{
		Error_Handler();
	}

	TIM2_IC_Init.ICFilter = 0;
	TIM2_IC_Init.ICPolarity = TIM_ICPOLARITY_RISING;
	TIM2_IC_Init.ICPrescaler = TIM_ICPSC_DIV1;
	TIM2_IC_Init.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if(HAL_TIM_IC_ConfigChannel(&TIM2_Handler, &TIM2_IC_Init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}


}


void LSE_Config(void)
{
	/*
	//This code is just here to remind you that don't ever do Oscillator config twice.
	//Do this all is the System_Clock_Config() After HAL_Init() Itself;
	RCC_OscInitTypeDef LSE_Init;

	LSE_Init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	LSE_Init.LSEState = RCC_LSE_ON;
	if(HAL_RCC_OscConfig(&LSE_Init) != HAL_OK)
	{
		Error_Handler();
	}
	*/

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(! is_captured)
	{
		if(count == 0)
		{
			IP_Capture_Count[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		}
		else if(count == 1)
		{
			IP_Capture_Count[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count = 0;
			is_captured = 1;
		}
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



