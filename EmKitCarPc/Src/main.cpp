/*
 * main.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */
#include "Hal.h"
#include "BmsKernel.h"

void _SendViaWiFi();
void _TurnNextOut();

int main(void)
{
	Hal::Init();
	BmsKernel::Init();

	Hal::LedBlue(true);
	Hal::LedRed(true);

	int32_t ticks = 0;

	uint8_t pwm1 = 1;
	uint8_t pwm2 = 128;

	while (true)
	{
		Hal::Tick();
		BmsKernel::Tick();

		if (Hal::GetSpendTicks(ticks) > Hal::GetTicksInSecond() / 2)
		{
			ticks = Hal::GetTickCount();
			//Hal::LedRed(!Hal::LedRed());
			//_SendViaWiFi();
			//_TurnNextOut();
			Hal::SetPwm(1, pwm1++);
			Hal::SetPwm(2, pwm2++);

			Hal::LedBlue(Hal::IsOptoTurnedIn(1));
			Hal::LedRed(Hal::IsOptoTurnedIn(2));
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

void _SendViaWiFi()
{
	char* sText = "Hello wifi!";
	Hal::UsartWiFi->Send((uint8_t*)sText, sizeof(sText));

	char rBuf[64];
	uint16_t received = Hal::UsartWiFi->Receive((uint8_t*)rBuf, sizeof(rBuf));
	if (received)
	{
		char* sTextRvd = "Hello wifi!";
		Hal::UsartWiFi->Send((uint8_t*)sTextRvd, sizeof(sTextRvd));
		Hal::UsartWiFi->Send((uint8_t*)rBuf, received);
	}
}



