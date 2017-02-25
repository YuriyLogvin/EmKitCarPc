/*
 * Hal.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */

#include "Hal.h"
#include <StmHAL.h>

void Hal::Init()
{
	StmHalInit();
}

void Hal::Tick()
{
	while (true)
	{
		StmHalTick();
	}
}

bool Hal::LedRed()
{
	return (HAL_GPIO_ReadPin(LED_RED_GPIO_Port, LED_RED_Pin) == GPIO_PIN_SET);
}

void Hal::LedRed(bool value)
{
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
}

bool Hal::LedBlue()
{
	return (HAL_GPIO_ReadPin(LED_BLUE_GPIO_Port, LED_BLUE_Pin) == GPIO_PIN_SET);
}

void Hal::LedBlue(bool value)
{
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
}


