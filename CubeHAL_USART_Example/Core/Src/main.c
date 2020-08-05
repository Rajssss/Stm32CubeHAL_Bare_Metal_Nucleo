/*
 * main.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Raj.S
 */


#include "stm32f4xx_hal.h"


void SystemClockConfig(void);


int main()
{
	HAL_Init();
	SystemClockConfig();


	return 0;
}



void SystemClockConfig(void)
{
	//No need here, as we will use default clock config.

}



void HAL_MspInit()
{
	//Low Level Initializations

	//Setup Priority Grouping

	//Enable Required exceptions

	//Configure Priority of the exceptions
}



