/*
 * EepromM95160.h
 *
 *  Created on: 28 бер. 2016
 *      Author: banz
 */

#ifndef EEPROMM95160_H_
#define EEPROMM95160_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

class EepromM95160
{
	uint8_t	_Status;
	SPI_HandleTypeDef* _SpiHandle;
	GPIO_TypeDef* _CsPort;
	GPIO_TypeDef* _WpPort;
	uint16_t _CsPin;
	uint16_t _WpPin;

	enum M95160Instructions
	{
		MiWriteEnable = 0x6,
		MiWriteDisable = 0x4,
		MiReadStatusRegister = 0x5,
		MiWriteStatusRegister = 0x1,
		MiReadFromMemoryArray = 0x3,
		MiWriteToMemoryArray = 0x2
	};

	int8_t _SendReceive(M95160Instructions instruction, uint8_t* dataToSend, uint16_t sizeToSend, uint8_t* dataToReceive, uint16_t sizeToReceive);
	int8_t _WriteMem(uint16_t addr, uint8_t* dataToWrite, uint16_t sizeToWrite);

public:
	EepromM95160(SPI_HandleTypeDef* spiHandle, GPIO_TypeDef* csPort, uint16_t csPin, GPIO_TypeDef* wpPort, uint16_t wpPin);

	enum M95160Statuses
	{
		MsNone = 0x0,
		MsWriteInProgress = 0x1,
		MsWriteEnableLatch = 0x2,
		MsBlockProtect0 = 0x4,
		MsBlockProtect1 = 0x8,
		MsStatusRegisterWriteProtect = 0x80
	};

	int8_t ReadStatus(M95160Statuses& status);
	int8_t WriteStatus();

	int8_t WriteEnable();
	int8_t WriteDisable();

	int8_t WriteMem(uint16_t addr, uint8_t* dataToWrite, uint16_t sizeToWrite);
	int8_t ReadMem(uint16_t addr, uint8_t* dataToRead, uint16_t sizeToRead);

};

#endif /* EEPROMM95160_H_ */
