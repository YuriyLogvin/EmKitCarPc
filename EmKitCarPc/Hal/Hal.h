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
#include "EepromM95160.h"
#include "EepromParameters.h"
#include "Mcp3208.h"

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

	static Stm32UsartDma* UsartWiFi;
	static Stm32UsartDma* UsartBms;
	static EepromM95160* Eeprom;
	static Mcp3208* AdcConverter1;
	static Mcp3208* AdcConverter2;

	static int8_t ReadParameterFromEeprom64(EepromParameters pName, int64_t& val);
	static int8_t ReadParameterFromEeprom32(EepromParameters pName, int32_t& val);
	static int8_t ReadParameterFromEeprom16(EepromParameters pName, int16_t& val);
	static int8_t ReadParameterFromEeprom8(EepromParameters pName, int8_t& val);

	static int8_t WriteParameterToEeprom64(EepromParameters pName, const int64_t pValue);
	static int8_t WriteParameterToEeprom32(EepromParameters pName, const int32_t pValue);
	static int8_t WriteParameterToEeprom16(EepromParameters pName, const int16_t pValue);
	static int8_t WriteParameterToEeprom8(EepromParameters pName, const int8_t pValue);

	static int32_t GetTickCount();
	static int32_t GetSpendTicks(int32_t fromTicks);
	static int32_t GetTicksDiff(int32_t fromTicks, int32_t toTicks);
	static short GetTicksInSecond();
	static short GetTicksInMilliSecond();

	static void SetPwm(uint8_t num, uint8_t pwm);

	static int16_t GetInputVoltage(uint8_t num);

	static int16_t GetTemperature(uint8_t num);

private:
	static void _UpdateDataFromExternalAdc();

};

#endif /* HAL_H_ */
