/*
 * EepromM95160.cpp
 *
 *  Created on: 28 бер. 2016
 *      Author: banz
 */

#include "EepromM95160.h"
#include <string.h>



void _Wait(uint16_t cicles)
{
	for (; cicles > 0; cicles-- )
	{
		continue;
	}
}

EepromM95160::EepromM95160(SPI_HandleTypeDef* spiHandle, GPIO_TypeDef* csPort, uint16_t csPin, GPIO_TypeDef* wpPort, uint16_t wpPin)
{
	_SpiHandle = spiHandle;
	_Status = 0;
	_CsPort = csPort;
	_WpPort = wpPort;
	_CsPin = csPin;
	_WpPin = wpPin;

}

int8_t EepromM95160::ReadStatus(M95160Statuses& status)
{

	uint8_t res = _SendReceive(MiReadStatusRegister, 0, 0, (uint8_t*)&status, 1);

	return res;
}

int8_t EepromM95160::WriteStatus()
{
	return -1;
}

int8_t EepromM95160::WriteEnable()
{
	return _SendReceive(MiWriteEnable, 0, 0, 0, 0);
}

int8_t EepromM95160::WriteDisable()
{
	return _SendReceive(MiWriteDisable, 0, 0, 0, 0);
}

int8_t EepromM95160::WriteMem(uint16_t addr, uint8_t* data, uint16_t size)
{
	/*Запись куском допускается внутри 32байтной страницы
	 * Внутри первой страницы дописать, а потом писать по 32 байта
	 */

	uint16_t pageNext = (addr / 32 + 1) * 32;
	uint8_t needWrite = pageNext - addr;

	for (;size > 0;)
	{
		if (needWrite > size)
			needWrite = size;

		int8_t res = _WriteMem(addr, data, needWrite);
		if (res)
			return res;

		size-=needWrite;
		data += needWrite;
		addr += needWrite;
		needWrite = 32;
	}

}

int8_t EepromM95160::_WriteMem(uint16_t addr, uint8_t* data, uint16_t size)
{
	uint8_t buff[34];

	if ((uint16_t)(2 + size) > sizeof(buff))
		return -1;

	buff[0] = addr >> 8;
	buff[1] = addr;

	memcpy(buff + 2, data, size);

	int8_t res = -2;

	EepromM95160::M95160Statuses status = MsNone;
	ReadStatus(status);
	while (status & MsWriteInProgress)
		ReadStatus(status);

	res = WriteEnable();
	if (res)
		return res;

	HAL_GPIO_WritePin(_WpPort, _WpPin, GPIO_PIN_RESET);

	res = _SendReceive(MiWriteToMemoryArray, buff, 2 + size, 0, 0);

	HAL_GPIO_WritePin(_WpPort, _WpPin, GPIO_PIN_SET);

	if (res)
		return res;

	res = WriteDisable();
	if (res)
		return res;

	return res;
}

int8_t EepromM95160::ReadMem(uint16_t addr, uint8_t* dataToRead, uint16_t sizeToRead)
{

	uint8_t addrBuff[2];
	addrBuff[0] = addr >> 8;
	addrBuff[1] = addr;

	EepromM95160::M95160Statuses status = MsNone;
	ReadStatus(status);
	while (status & MsWriteInProgress)
		ReadStatus(status);

	int8_t res =
		_SendReceive(MiReadFromMemoryArray, addrBuff, sizeof(addrBuff), dataToRead, sizeToRead);

	return res;

}

int8_t EepromM95160::_SendReceive(M95160Instructions instruction, uint8_t* dataToSend, uint16_t sizeToSend, uint8_t* dataToReceive, uint16_t sizeToReceive)
{
	/* Отправляем код инструкции + данные(как правило адрес),
	 * сразу после них будет в тот же буфер записан ответ. *Не в начале буфера.
	 * */
	uint8_t instr[34];

	if ((uint16_t)(1 + sizeToSend + sizeToReceive) > sizeof(instr))
		return -1;

	instr[0] = instruction;

	if (sizeToSend)
		memcpy(instr + 1, dataToSend, sizeToSend);

	HAL_GPIO_WritePin(_CsPort, _CsPin, GPIO_PIN_RESET);

	HAL_StatusTypeDef status =
		HAL_SPI_TransmitReceive(_SpiHandle, instr, instr, 1 + sizeToSend + sizeToReceive, 100);

	HAL_GPIO_WritePin(_CsPort, _CsPin, GPIO_PIN_SET);

	if (status == HAL_OK)
	{
		if (sizeToReceive)
			memcpy(dataToReceive, instr + 1 + sizeToSend, sizeToReceive);
	}

	return status;

}

