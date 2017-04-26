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
			//Hal::LedRed(!Hal::LedRed());
			//_TurnNextOut();
			Hal::SetPwm(1, pwm1++);
			Hal::SetPwm(2, pwm2++);

			Hal::LedBlue(Hal::IsOptoTurnedIn(1));
			Hal::LedRed(Hal::IsOptoTurnedIn(2));

			Hal::UsartRs485->Send("1234\n\r");

			unsigned char rxData[100];
			int16_t received = Hal::UsartWiFi->Receive(rxData, sizeof(rxData));
			if (received)
			{
				_ProcessInputData(rxData, received);
				//Hal::UsartWiFi->Send("Received %s\n", rxData);
			}


			//Temperature
			/*Hal::UsartWiFi->Send(
								"1 - %x;"
								"2 - %x; "
								"3 - %x; "
								"4 - %x; "
								"\n",
								Hal::GetTemperature(1),
								Hal::GetTemperature(2),
								Hal::GetTemperature(3),
								Hal::GetTemperature(4));
								*/

			//Voltages
			/*Hal::UsartWiFi->Send(
					"1 - %x;"
					"2 - %x; "
					"3 - %x; "
					"4 - %x; "
					"5 - %x; "
					"6 - %x; "
					"7 - %x; "
					"8 - %x; "
					"9 - %x; "
					"10 - %x; "
					"11 - %x; "
					"12 - %x; "
					"\n",
					Hal::GetInputVoltage(1),
					Hal::GetInputVoltage(2),
					Hal::GetInputVoltage(3),
					Hal::GetInputVoltage(4),
					Hal::GetInputVoltage(5),
					Hal::GetInputVoltage(6),
					Hal::GetInputVoltage(7),
					Hal::GetInputVoltage(8),
					Hal::GetInputVoltage(9),
					Hal::GetInputVoltage(10),
					Hal::GetInputVoltage(11),
					Hal::GetInputVoltage(12));
					*/
			//Hal::UsartWiFi->Send("%x\n", Hal::GetInputVoltage(3));
			//int8_t ii = 0;
			//Hal::ReadParameterFromEeprom8(epBmsCellCount, ii);
			//Hal::UsartWiFi->Send("%x\n", ii);
			//ii++;

			//Hal::WriteParameterToEeprom8(epBmsCellCount, ii);

		}


	}

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


