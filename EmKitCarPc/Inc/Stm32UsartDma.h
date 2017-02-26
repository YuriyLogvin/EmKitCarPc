/*
 * Stm32UsartRecever.h
 *
 *  Created on: Feb 26, 2017
 *      Author: banz
 */

#ifndef STM32USARTDMA_H_
#define STM32USARTDMA_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"


class Stm32UsartDma {
	uint16_t _RcvBufSize;
	uint16_t _SendBufSize;
	uint8_t* _UsartRecvBuff;
	uint8_t* _UsartSendBuff;
	uint16_t  _UsartReceivePos;
	UART_HandleTypeDef* _Huart;
public:
	Stm32UsartDma(UART_HandleTypeDef* huart, uint16_t rcvBufSize, uint16_t sendBufSize);
	~Stm32UsartDma();

	bool Send(uint8_t* data, uint16_t dataLen);
	uint16_t Receive(uint8_t* data, uint16_t dataBufSize);
};

#endif /* STM32USARTDMA_H_ */
