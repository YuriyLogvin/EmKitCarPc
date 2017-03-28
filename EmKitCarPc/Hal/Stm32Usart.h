/*
 * Stm32Usart.h
 *
 *  Created on: Mar 28, 2017
 *      Author: banz
 */

#ifndef STM32USART_H_
#define STM32USART_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define _RxTimeout 10
#define _TxTimeout 10

class Stm32Usart {
	UART_HandleTypeDef* _Huart;
	GPIO_TypeDef* _TxEnPort;
	uint16_t _TxEnPin;
public:
	Stm32Usart(UART_HandleTypeDef* huart);
	Stm32Usart(UART_HandleTypeDef* huart, GPIO_TypeDef* txEnPort, uint16_t txEnPin);
	~Stm32Usart();

	bool Send(uint8_t* data, uint16_t dataLen);
	bool Send(const char* format, ...);
	uint16_t Receive(uint8_t* data, uint16_t dataBufSize);

};

#endif /* STM32USART_H_ */
