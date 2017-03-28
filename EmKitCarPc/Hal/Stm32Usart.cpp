/*
 * Stm32Usart.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: banz
 */

#include "Stm32Usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

Stm32Usart::Stm32Usart(UART_HandleTypeDef* huart) {
	_Huart = huart;
	_TxEnPort = 0;
	_TxEnPin = 0;
}

Stm32Usart::Stm32Usart(UART_HandleTypeDef* huart, GPIO_TypeDef* txEnPort, uint16_t txEnPin) {
	_Huart = huart;
	_TxEnPort = txEnPort;
	_TxEnPin = txEnPin;

	if (_TxEnPort)
		HAL_GPIO_WritePin(_TxEnPort, _TxEnPin, GPIO_PIN_RESET);
}

Stm32Usart::~Stm32Usart() {
	// TODO Auto-generated destructor stub
}


bool Stm32Usart::Send(const char* format, ...)
{
	if (_Huart->State & HAL_UART_STATE_BUSY_TX & 0xf0)
		return false;

	char sendBuff[128];

	va_list args;
	va_start(args, format);
	vsnprintf(sendBuff, sizeof(sendBuff), format, args);
	va_end(args);

	if (_TxEnPort)
		HAL_GPIO_WritePin(_TxEnPort, _TxEnPin, GPIO_PIN_SET);

	HAL_UART_Transmit(_Huart, (uint8_t*)sendBuff, strlen(sendBuff), _TxTimeout);

	if (_TxEnPort)
		HAL_GPIO_WritePin(_TxEnPort, _TxEnPin, GPIO_PIN_RESET);

}

bool Stm32Usart::Send(uint8_t* data, uint16_t dataLen)
{
	if (_Huart->State & HAL_UART_STATE_BUSY_TX & 0xf0)
		return false;

	if (_TxEnPort)
		HAL_GPIO_WritePin(_TxEnPort, _TxEnPin, GPIO_PIN_SET);

	HAL_UART_Transmit(_Huart, data, dataLen, _TxTimeout);

	if (_TxEnPort)
		HAL_GPIO_WritePin(_TxEnPort, _TxEnPin, GPIO_PIN_RESET);

	return true;
}

uint16_t Stm32Usart::Receive(uint8_t* data, uint16_t dataBufSize)
{
	uint16_t i = 0;

	for (; i < dataBufSize;i++)
	{
		HAL_StatusTypeDef status = HAL_UART_Receive(_Huart, data + i, 1, _RxTimeout);
		if (status == HAL_TIMEOUT)
		{
			return i;
		}
	}

	return i;
}

