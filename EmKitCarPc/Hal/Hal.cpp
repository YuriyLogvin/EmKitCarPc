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
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim2;
uint32_t HAL_GetTick(void);
void HAL_IncTick(void);
}

static uint32_t _HalTicker;

static uint32_t _uwTick;
static uint8_t _uwTickSub;

void HAL_IncTick(void)
{
	_uwTickSub++;
	if (_uwTickSub > 9)
	{
		_uwTick++;
		_uwTickSub = 0;
	}

	_HalTicker++;
}

uint32_t HAL_GetTick(void)
{
	return _uwTick;
}

Stm32UsartDma* Hal::UsartWiFi;
Stm32UsartDma* Hal::UsartBms;
Stm32Usart* Hal::UsartRs485;

EepromM95160* Hal::Eeprom;
Mcp3208* Hal::AdcConverter1;
Mcp3208* Hal::AdcConverter2;


void Hal::Init()
{
	StmHalInit();

	UsartWiFi = new Stm32UsartDma(&huart1, 0x200, 0x100);
	UsartBms = new Stm32UsartDma(&huart4, 0x200, 0x100);
	UsartRs485 = new Stm32Usart(&huart5, Rs485TxEn_GPIO_Port, Rs485TxEn_Pin);;
	Eeprom = new EepromM95160(&hspi2, SPI2_CS_MEM_GPIO_Port, SPI2_CS_MEM_Pin, SPI2_WP_GPIO_Port, SPI2_WP_Pin);
	AdcConverter1 = new Mcp3208(&hspi2, SPI2_CS_ADC1_GPIO_Port, SPI2_CS_ADC1_Pin, true);
	AdcConverter2 = new Mcp3208(&hspi2, SPI2_CS_ADC2_GPIO_Port, SPI2_CS_ADC2_Pin, true);

	HAL_GPIO_WritePin(SPI2_HOLD_GPIO_Port, SPI2_HOLD_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(SPI2_HOLD_GPIO_Port, SPI2_HOLD_Pin, GPIO_PIN_SET);

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

}

void Hal::Tick()
{
	StmHalTick();

	_UpdateDataFromExternalAdc();
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

/* ¬се параметры хран€тс€ в Eeprom начина€ с нулевого адреса
 *  аждый параметр занимает 64 бита, независимо от реального размера
 * */
int8_t Hal::ReadParameterFromEeprom64(EepromParameters pName, int64_t& val)
{
	uint16_t addr = pName * 8;
	return Eeprom->ReadMem(addr, (uint8_t*)&val, sizeof(val));
}

int8_t Hal::ReadParameterFromEeprom32(EepromParameters pName, int32_t& val)
{
	uint16_t addr = pName * 8;
	return Eeprom->ReadMem(addr, (uint8_t*)&val, sizeof(val));
}

int8_t Hal::ReadParameterFromEeprom16(EepromParameters pName, int16_t& val)
{
	uint16_t addr = pName * 8;
	return Eeprom->ReadMem(addr, (uint8_t*)&val, sizeof(val));
}

int8_t Hal::ReadParameterFromEeprom8(EepromParameters pName, int8_t& val)
{
	uint16_t addr = pName * 8;
	return Eeprom->ReadMem(addr, (uint8_t*)&val, sizeof(val));
}

int8_t Hal::WriteParameterToEeprom64(EepromParameters pName, const int64_t pValue)
{
	int64_t existVal = 0;
	ReadParameterFromEeprom64(pName, existVal);
	if (existVal == pValue)
		return 0;
	uint16_t addr = pName * 8;
	return Eeprom->WriteMem(addr, (uint8_t*)&pValue, sizeof(pValue));
}

int8_t Hal::WriteParameterToEeprom32(EepromParameters pName, const int32_t pValue)
{
	int32_t existVal = 0;
	ReadParameterFromEeprom32(pName, existVal);
	if (existVal == pValue)
		return 0;
	uint16_t addr = pName * 8;
	return Eeprom->WriteMem(addr, (uint8_t*)&pValue, sizeof(pValue));
}

int8_t Hal::WriteParameterToEeprom16(EepromParameters pName, const int16_t pValue)
{
	int16_t existVal = 0;
	ReadParameterFromEeprom16(pName, existVal);
	if (existVal == pValue)
		return 0;
	uint16_t addr = pName * 8;
	return Eeprom->WriteMem(addr, (uint8_t*)&pValue, sizeof(pValue));
}

int8_t Hal::WriteParameterToEeprom8(EepromParameters pName, const int8_t pValue)
{
	int8_t existVal = 0;
	ReadParameterFromEeprom8(pName, existVal);
	if (existVal == pValue)
		return 0;
	uint16_t addr = pName * 8;
	return Eeprom->WriteMem(addr, (uint8_t*)&pValue, sizeof(pValue));
}

int32_t Hal::GetTickCount()
{
	return _HalTicker;
}

int32_t Hal::GetTicksDiff(int32_t fromTicks, int32_t toTicks)
{
	if (fromTicks <= toTicks)
		return toTicks - fromTicks;

	return (0xffffffff - fromTicks + toTicks);
}

int32_t Hal::GetSpendTicks(int32_t fromTicks)
{
	int32_t nowT = GetTickCount();
	if (nowT >= fromTicks)
		return nowT - fromTicks;

	return (0xffffffff - fromTicks + nowT);
	//return embedded_get_spent_ms(fromTicks);
}

short Hal::GetTicksInSecond()
{
	return 10000;
}

short Hal::GetTicksInMilliSecond()
{
	return 10;
}

void Hal::SetPwm(uint8_t num, uint8_t pwm)
{
	switch (num)
	{
	case 1:
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, pwm << 2);
		break;
	case 2:
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, pwm << 2);
		break;
	}
}

uint16_t _AdcInputs[12];
uint16_t _AdcTempSensors[4];

void Hal::_UpdateDataFromExternalAdc()
{
	_AdcInputs[0] = AdcConverter1->GetAdc(7);
	_AdcInputs[1] = AdcConverter1->GetAdc(6);
	_AdcInputs[2] = AdcConverter1->GetAdc(5);
	_AdcInputs[3] = AdcConverter1->GetAdc(4);
	_AdcInputs[4] = AdcConverter1->GetAdc(3);
	_AdcInputs[5] = AdcConverter1->GetAdc(2);
	_AdcInputs[6] = AdcConverter2->GetAdc(7);
	_AdcInputs[7] = AdcConverter2->GetAdc(6);
	_AdcInputs[8] = AdcConverter2->GetAdc(5);
	_AdcInputs[9] = AdcConverter2->GetAdc(4);
	_AdcInputs[10] = AdcConverter2->GetAdc(3);
	_AdcInputs[11] = AdcConverter2->GetAdc(2);


	_AdcTempSensors[0] = AdcConverter1->GetAdc(1);
	_AdcTempSensors[1] = AdcConverter1->GetAdc(0);
	_AdcTempSensors[2] = AdcConverter2->GetAdc(1);
	_AdcTempSensors[3] = AdcConverter2->GetAdc(0);


}

int16_t Hal::GetInputVoltage(uint8_t num)
{
	int16_t res = 0;

	if (num > 12)
		return -1;
	if (num < 1)
		return -2;

	res = _AdcInputs[num - 1];


	return res;
}

int16_t Hal::GetTemperature(uint8_t num)
{
	if (num > 4)
		return -1;
	if (num < 1)
		return -2;

	return _AdcTempSensors[num - 1];
}

