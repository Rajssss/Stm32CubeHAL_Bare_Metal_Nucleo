/*
 * main.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 *      Description: This Example uses a general purpose timer (TIM2) in Output Capture PWM mode and
 *      			 Generates PWM Signal of time period 1 Sec and of variable duty cycle to first
 *      			 increase the brightness of LED then decrease it.
 *
 */

#include "main.h"


#define DUTY_25		25U
#define DUTY_45		45U
#define DUTY_75		75U
#define DUTY_95		95U


TIM_HandleTypeDef TIM2_Handler;
UART_HandleTypeDef UART2Init;


int main(void)
{
	uint16_t brightness = 0;

	HAL_Init();

	System_Clock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);		//Increases Accuracy

	GIOP_LED_Init();

	UART2_Init();

	TIMER2_Init();

	if(HAL_TIM_PWM_Start(&TIM2_Handler, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	while(1)
	{
		while(brightness < TIM2_Handler.Init.Period)
		{
			brightness+=20;
			__HAL_TIM_SET_COMPARE(&TIM2_Handler, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);		//1ms Delay to hold the Processor
		}
		while(brightness > 0)
		{
			brightness-=20;
			__HAL_TIM_SET_COMPARE(&TIM2_Handler, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);		//1ms Delay to hold the Processor
		}

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
	TIM_OC_InitTypeDef TIM2_PWM_Init;

	memset(&TIM2_PWM_Init, 0, sizeof(TIM2_PWM_Init));

	//Initialize TIM2 for 1Sec Time base
	TIM2_Handler.Instance = TIM2;
	TIM2_Handler.Init.Period = 10000U-1U;
	TIM2_Handler.Init.Prescaler = 4U;
	if(HAL_TIM_PWM_Init(&TIM2_Handler) != HAL_OK)
	{
		Error_Handler();
	}

	//Initialize PWM
	TIM2_PWM_Init.OCMode = TIM_OCMODE_PWM1;
	TIM2_PWM_Init.OCPolarity = TIM_OCPOLARITY_HIGH;

	//Start with 0 duty cycle ie pulse=0, So Off state.
	TIM2_PWM_Init.Pulse = 0;
	if(HAL_TIM_PWM_ConfigChannel(&TIM2_Handler, &TIM2_PWM_Init, TIM_CHANNEL_1) != HAL_OK)
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



