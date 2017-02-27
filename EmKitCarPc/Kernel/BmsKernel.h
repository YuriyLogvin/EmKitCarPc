/*
 * BmsKernel.h
 *
 *  Created on: 1 груд. 2016
 *      Author: banz
 */

#ifndef BMSKERNEL_H_
#define BMSKERNEL_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void BmsKernelInit();
void BmsKernelTick();
//void PwKernelSetSendDataHandler(int (*sendDataHandler)(const uint8_t* buff, const uint16_t len));
//void PwKernelOnReceiveData(const uint8_t* buff, const uint16_t len);

void BmsReceiveData(uint8_t data);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

enum VoltageLevels
{
	vlMinVoltage,
	vlMaxVoltage,
	vlBalansing,
	vlCharged
};

enum InputSources
{
	isNone = 0,
	isCharger = 1,
	isIgnition = 2
};

enum OutTypes
{
	otNone = 0,
	otCharger = 1,
	otIgnition = 2
};

enum BmsActiveModes
{
	bamNone = 0,
	bamCharging = 1,
	bamDischarging = 2,
	bamErrLostConnection = 11
};

struct CellInfo
{
	uint8_t Num;
	int16_t Voltage;
	uint8_t State;
	int16_t Thermo;
	int16_t ThermoBal;
	uint8_t Ver;
	bool operator==(const CellInfo& other) const
	{
		if (Num != other.Num)
			return false;
		if (Voltage != other.Voltage)
			return false;
		if (State != other.State)
			return false;
		if (Thermo != other.Thermo)
			return false;
		if (ThermoBal != other.ThermoBal)
			return false;
		if (Ver != other.Ver)
			return false;
		return true;
	}

	bool operator!=(const CellInfo& other) const
	{
		return ! operator==(other);
	}
};

typedef void (*ReceivedCellStateCallBack) (CellInfo& cellInfo);

class BmsKernel
{
	static void _ResetCellCount(uint8_t cellCount);
	static void _DoBallancing(uint8_t cellNum, int16_t cellVoltage);
	static void _TurnOnBallansing(uint8_t cellNum);
	static void _SendRequestForExternalModules();
	static void _SendCalibrateForExternalModules(uint8_t cellNum, int16_t cellVoltage);
	static void _SendData();
	static void _TryReceiveCellState();
	static void _OnReceivedCellState(CellInfo& cellInfo);
	static BmsActiveModes _GetActiveMode();

public:
	static void Init();
	static void Tick();

	static BmsActiveModes GetActiveMode();

	static void SetReceivedCellStateCallBack(ReceivedCellStateCallBack receivedCellState);

	static void OnReceiveDataFromBms(uint8_t data);

	static void SetCellCount(uint8_t cellCount);
	static uint8_t GetCellCount();

	static void SetVoltageLevel(VoltageLevels level, int16_t value);
	static int16_t GetVoltageLevel(VoltageLevels level);

	static int16_t GetCellVoltage(int8_t cellNum);

	static InputSources GetInputSource(int8_t sNum /*1-4*/);
	static void SetInputSource(int8_t sNum /*1-4*/, InputSources value);

	static OutTypes GetOutputType(int8_t oNum /*1-4*/);
	static void SetOutputType(int8_t oNum /*1-4*/, OutTypes value);

	static void CalibrateVoltage(uint8_t cellNum, int16_t cellVoltage);

	static bool IgnitionIsOn();
	static bool ChargerIsOn();
};

#endif //#ifdef __cplusplus

#endif /* BMSKERNEL_H_ */
