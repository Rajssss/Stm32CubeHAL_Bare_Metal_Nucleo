/*
 * main.c
 *
 *  Created on: Aug 10, 2020
 *      Author: Rajssss@GitHub.com
 */

#include "main.h"


void main(void)
{
	HAL_Init();

	System_Clock_Config();

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
