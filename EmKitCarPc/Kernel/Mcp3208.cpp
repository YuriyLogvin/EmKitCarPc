/*
 * Mcp3208.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: banz
 */

#include <Mcp3208.h>

Mcp3208::Mcp3208(SPI_HandleTypeDef* spiHandle, GPIO_TypeDef* csPort, uint16_t csPin, bool singleMode)
{
	_SpiHandle = spiHandle;
	_CsPort = csPort;
	_CsPin = csPin;
	_SingleMode = singleMode;
	HAL_GPIO_WritePin(_CsPort, _CsPin, GPIO_PIN_SET);
}

Mcp3208::~Mcp3208() {
}

uint16_t Mcp3208::GetAdc(uint8_t chNum)
{
	HAL_GPIO_WritePin(_CsPort, _CsPin, GPIO_PIN_RESET);

	uint8_t txData[3] = {0};
	uint8_t rxData[3] = {0};

	txData[0] = 0x10; //Start bit

	if (_SingleMode)
		txData[0] |= 0x8;

	txData[0] |= (chNum & 0x7);

	HAL_StatusTypeDef status =
		HAL_SPI_TransmitReceive(_SpiHandle, txData, rxData, sizeof(txData), 100);

	HAL_GPIO_WritePin(_CsPort, _CsPin, GPIO_PIN_SET);

	uint16_t res = 0;
	res = (rxData[1] & 0x3f) << 6;
	res |= (rxData[2] & 0xfc) >> 2;

	return res;
}

