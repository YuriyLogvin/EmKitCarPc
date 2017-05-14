/*
 * Stm32UsartRecever.cpp
 *
 *  Created on: Feb 26, 2017
 *      Author: banz
 */

#include "Stm32UsartDma.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

Stm32UsartDma::Stm32UsartDma(UART_HandleTypeDef* huart, uint16_t rcvBufSize, uint16_t sendBufSize)
{
	_Huart = huart;
	_RcvBufSize = rcvBufSize;
	_SendBufSize = sendBufSize;

	_UsartSendBuff = (uint8_t*)malloc(_SendBufSize);
	memset(_UsartSendBuff, 0xff, _SendBufSize);

	_UsartReceivePos = 0;
	_UsartRecvBuff = (uint8_t*)malloc(_RcvBufSize*2);
	memset(_UsartRecvBuff, 0xff, _RcvBufSize*2);
	HAL_UART_Receive_DMA(_Huart, _UsartRecvBuff, _RcvBufSize);
}

Stm32UsartDma::~Stm32UsartDma() {
	free(_UsartSendBuff);
	free(_UsartRecvBuff);
}

bool Stm32UsartDma::Send(const char* format, ...)
{
	if (_Huart->State & HAL_UART_STATE_BUSY_TX & 0xf0)
		return false;

	va_list args;
	va_start(args, format);
	vsnprintf((char*)_UsartSendBuff, _SendBufSize, format, args);
	va_end(args);

	HAL_UART_Transmit_DMA(_Huart, _UsartSendBuff, strlen((char*)_UsartSendBuff));

}

bool Stm32UsartDma::Send(uint8_t* data, uint16_t dataLen)
{
	if (_Huart->State & HAL_UART_STATE_BUSY_TX & 0xf0)
		return false;

	if (dataLen > _SendBufSize)
		return false;

	memcpy(_UsartSendBuff, data, dataLen);
	HAL_UART_Transmit_DMA(_Huart, _UsartSendBuff, dataLen);
	return true;
}

uint16_t Stm32UsartDma::Receive(uint8_t* data, uint16_t dataBufSize)
{
	uint8_t received = 0;
	for (;1;)
	{
		if (_UsartRecvBuff[_UsartReceivePos*2+1] == 0xff)
			break;

		data[received++] = _UsartRecvBuff[_UsartReceivePos*2];

		_UsartRecvBuff[_UsartReceivePos*2+1] = 0xff;

		_UsartReceivePos++;
		if (_UsartReceivePos >= _RcvBufSize )
			_UsartReceivePos = 0;

		if (received >= dataBufSize)
			break;

	}

	return received;
}

