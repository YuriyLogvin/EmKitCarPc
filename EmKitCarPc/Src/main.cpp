/*
 * main.cpp
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */

#include <StmHAL.h>

int main(void)
{
	StmHalInit();

	while (true)
	{
		StmHalTick();
	}
}


