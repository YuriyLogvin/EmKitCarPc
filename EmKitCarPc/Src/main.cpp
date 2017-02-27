/*
 * main.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */
#include "Hal.h"
#include "BmsKernel.h"

int main(void)
{
	Hal::Init();
	BmsKernel::Init();

	Hal::LedBlue(true);
	Hal::LedRed(true);

	int32_t ticks = 0;


	while (true)
	{
		Hal::Tick();
		BmsKernel::Tick();

		if (Hal::GetSpendTicks(ticks) > Hal::GetTicksInSecond() * 2)
		{
			ticks = Hal::GetTickCount();
			Hal::LedRed(!Hal::LedRed());
		}

	}

}


