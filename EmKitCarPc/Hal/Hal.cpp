/*
 * Hal.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */

#include "Hal.h"
#include <StmHAL.h>
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C"
{
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
}

Stm32UsartDma Hal::UsartWiFi(&huart1, 0x200, 0x100);
Stm32UsartDma Hal::UsartBms(&huart4, 0x200, 0x100);

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

bool Hal::IsTurnedOut(uint8_t outNum)
{
	switch (outNum)
	{
	case 1:
		return (HAL_GPIO_ReadPin(GPIO_Out1_GPIO_Port, GPIO_Out1_Pin) == GPIO_PIN_SET);
	case 2:
		return (HAL_GPIO_ReadPin(GPIO_Out2_GPIO_Port, GPIO_Out2_Pin) == GPIO_PIN_SET);
	case 3:
		return (HAL_GPIO_ReadPin(GPIO_Out3_GPIO_Port, GPIO_Out3_Pin) == GPIO_PIN_SET);
	case 4:
		return (HAL_GPIO_ReadPin(GPIO_Out4_GPIO_Port, GPIO_Out4_Pin) == GPIO_PIN_SET);
	case 5:
		return (HAL_GPIO_ReadPin(GPIO_Out5_GPIO_Port, GPIO_Out5_Pin) == GPIO_PIN_SET);
	case 6:
		return (HAL_GPIO_ReadPin(GPIO_Out6_GPIO_Port, GPIO_Out6_Pin) == GPIO_PIN_SET);
	}

	return false;
}

void Hal::TurnOut(uint8_t outNum, bool value)
{
	switch (outNum)
	{
	case 1:
		HAL_GPIO_WritePin(GPIO_Out1_GPIO_Port, GPIO_Out1_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIO_Out2_GPIO_Port, GPIO_Out2_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIO_Out3_GPIO_Port, GPIO_Out3_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIO_Out4_GPIO_Port, GPIO_Out4_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIO_Out5_GPIO_Port, GPIO_Out5_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIO_Out6_GPIO_Port, GPIO_Out6_Pin, value?GPIO_PIN_SET:GPIO_PIN_RESET);
		break;
	}
}

bool Hal::IsTurnedIn(uint8_t inpNum)
{
	return false;
}

bool Hal::IsOptoTurnedIn(uint8_t inpNum)
{
	switch (inpNum)
	{
	case 1:
		return (HAL_GPIO_ReadPin(InpOpto1_GPIO_Port, InpOpto1_Pin) == GPIO_PIN_SET);
	case 2:
		return (HAL_GPIO_ReadPin(InpOpto2_GPIO_Port, InpOpto2_Pin) == GPIO_PIN_SET);
	}
	return false;
}

