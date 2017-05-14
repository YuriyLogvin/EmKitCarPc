/*
 * main.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */
#include "Hal.h"
#include "BmsKernel.h"
#include <stdio.h>
#include <string.h>

#include "Shell.h"

void _ProcessInputData(uint8_t* data, uint16_t dataBufSize);
void _ShellInit();

void CellStateCallBack(CellInfo& cellInfo);

int main(void)
{
	Hal::Init();
	BmsKernel::Init();

	Shell::Init();

	Hal::LedBlue(true);
	Hal::LedRed(true);

	int32_t ticks = 0;
	int32_t halTicks = 0;

	uint8_t pwm1 = 1;
	uint8_t pwm2 = 128;

	BmsKernel::SetReceivedCellStateCallBack(CellStateCallBack);

	//BmsKernel::TransmitCellState(true);
	Hal::BmsEnabled(true);
	//BmsKernel::SetCellCount(6);

	while (true)
	{
		if (Hal::GetSpendTicks(halTicks) > Hal::GetTicksInMilliSecond() * 2)
		{
			halTicks = Hal::GetTickCount();
			Hal::Tick();
		}

		BmsKernel::Tick();


		if (Hal::GetSpendTicks(ticks) > Hal::GetTicksInSecond())
		{
			ticks = Hal::GetTickCount();

			unsigned char rxData[100];
			int16_t received = Hal::UsartWiFi->Receive(rxData, sizeof(rxData));
			if (received)
			{
				_ProcessInputData(rxData, received);
			}
		}


	}

}

void _SendViaWiFi(const char* str)
{
	while (!Hal::UsartWiFi->Send((uint8_t*)str, strlen(str)));
}

void CellStateCallBack(CellInfo& cellInfo)
{
	if (!BmsKernel::TransmitCellState())
		return;

	char buff[64];
	sprintf(buff, "\tCell %i: %i, %i, %i\r\n", cellInfo.Num, cellInfo.Voltage, cellInfo.Thermo, cellInfo.State);

	_SendViaWiFi(buff);
}

char _CommandBuff[32];
int8_t _CommandBuffPos = 0;
void _ProcessInputData(uint8_t* data, uint16_t dataBufSize)
{
	bool receivedCommand = false;
	for (uint16_t i = 0; i < dataBufSize; i++)
	{
		if (data[i] == '\r')
			continue;

		if (data[i] == '\n')
		{
			receivedCommand = true;
			break;
		}

		if (_CommandBuffPos + 2 >= sizeof(_CommandBuff))
		{
			_CommandBuffPos = 0;
			return;
		}

		_CommandBuff[_CommandBuffPos++] = data[i];
	}

	if (!receivedCommand)
		return;

	_CommandBuff[_CommandBuffPos] = 0;
	_CommandBuffPos = 0;

	Shell::ProcessCommand(_CommandBuff);


}


