/*
 * Mcp3208.h
 *
 *  Created on: Mar 25, 2017
 *      Author: banz
 */

#ifndef MCP3208_H_
#define MCP3208_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

class Mcp3208 {
	SPI_HandleTypeDef* _SpiHandle;
	GPIO_TypeDef* _CsPort;
	uint16_t _CsPin;
	bool _SingleMode;

public:
	Mcp3208(SPI_HandleTypeDef* spiHandle, GPIO_TypeDef* csPort, uint16_t csPin, bool singleMode);
	~Mcp3208();

	uint16_t GetAdc(uint8_t chNum);


};

#endif /* MCP3208_H_ */
