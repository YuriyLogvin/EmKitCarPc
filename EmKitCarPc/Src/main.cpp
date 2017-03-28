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

//void _SendViaWiFi(const char* val);
void _SendViaWiFi(const char* format, ...);
void _TurnNextOut();

int main(void)
{
	Hal::Init();
	BmsKernel::Init();

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
			int16_t received = Hal::UsartRs485->Receive(rxData, sizeof(rxData));
			if (received)
			{
				rxData[received] = 0;
				Hal::UsartWiFi->Send("Received %s\n", rxData);
			}
			/*
			HAL_StatusTypeDef s = HAL_UART_Receive(&huart5, rData, sizeof(rData), 10);
			if (s != HAL_TIMEOUT)
			{
				Hal::UsartWiFi->Send("Received %s\n", rData);
			}*/

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

int _ActiveOut = 0;
void _TurnNextOut()
{
	switch (++_ActiveOut)
	{
	case 1:
		Hal::TurnOut(6, false);
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		Hal::TurnOut(_ActiveOut - 1, false);
		Hal::TurnOut(_ActiveOut, true);
		break;
	default:
		_ActiveOut = 0;
		break;
	}
}

void _SendViaWiFi(const char* format, ...)
{

    //Hal::UsartWiFi->Send((uint8_t*)_SendWfBuff, strlen(_SendWfBuff));

	/*char rBuf[64];
	uint16_t received = Hal::UsartWiFi->Receive((uint8_t*)rBuf, sizeof(rBuf));
	if (received)
	{
		char* sTextRvd = "Received: \n";
		Hal::UsartWiFi->Send((uint8_t*)sTextRvd, strlen(sTextRvd));
		Hal::UsartWiFi->Send((uint8_t*)rBuf, received);
		char* sTextRN = "\n\r";
		Hal::UsartWiFi->Send((uint8_t*)sTextRN, strlen(sTextRN));
	}
	*/
}



