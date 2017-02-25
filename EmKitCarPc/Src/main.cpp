/*
 * main.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */
#include "Hal.h"

int main(void)
{
	Hal::Init();

	Hal::LedBlue(true);
	Hal::LedRed(true);

	while (true)
	{
		Hal::Tick();
	}

}


