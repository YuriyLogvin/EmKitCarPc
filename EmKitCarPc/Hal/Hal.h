/*
 * Hal.h
 *
 *  Created on: Feb 25, 2017
 *      Author: banz
 */

#ifndef HAL_H_
#define HAL_H_

#include <stdint.h>
#include "Stm32UsartDma.h"

class Hal {
public:
	static void Init();
	static void Tick();

	static bool LedRed();
	static void LedRed(bool);
	static bool LedBlue();
	static void LedBlue(bool);

	static bool IsTurnedOut(uint8_t outNum);
	static void TurnOut(uint8_t outNum, bool value);

	static bool IsTurnedIn(uint8_t inpNum);

	static bool IsOptoTurnedIn(uint8_t inpNum);

	static Stm32UsartDma UsartWiFi;
	static Stm32UsartDma UsartBms;

};

#endif /* HAL_H_ */
