/*
 * BmsKernel.cpp
 *
 *  Created on: 1 ����. 2016
 *      Author: banz
 */

#include "BmsKernel.h"
#include "Hal.h"
#include "stdlib.h"
#include "ReceiveBuffer.h"
#include "ReceiveMetodHost.h"
#include "SendMetodHost.h"
#include "ProtocolHost.h"
#include "InterfaceMetods.h"

void BmsKernelInit()
{
	BmsKernel::Init();
}

void BmsKernelTick()
{
	BmsKernel::Tick();
}

int16_t _BmsLevelMin = 0;
int16_t _BmsLevelMax = 0;
int16_t _BmsLevelBallancing = 0;
int16_t _BmsLevelBallancingStart = 0;
int16_t _BmsLevelCharged = 0;
int16_t _BmsBallancingDelta = 10;

InputSources _Input1Source = isNone;
InputSources _Input2Source = isNone;
InputSources _Input3Source = isNone;
InputSources _Input4Source = isNone;

OutTypes _Out1Type = otNone;
OutTypes _Out2Type = otNone;
OutTypes _Pwm1Type = otNone;
OutTypes _Pwm2Type = otNone;

static ProtocolHost* _BmsProtocolHost = 0;
static ReceiveMetodHost* _BmsReceiveMetodHost = 0;
static SendMetodHost* _BmsSendMetodHost = 0;
//static ReceiveBuffer* _BmsReceiveBuffer = 0;

uint8_t _CellCount = 0;
uint8_t _BmsConnectState[8]; // 8 * 8 == 256 cells

int16_t* _CellVoltages = 0;
BmsActiveModes _BmsActiveMode = bamCharging;



void BmsKernel::Init()
{
	Hal::ReadParameterFromEeprom16(epBmsLevelMin, _BmsLevelMin);
	Hal::ReadParameterFromEeprom16(epBmsLevelMax, _BmsLevelMax);
	Hal::ReadParameterFromEeprom16(epBmsLevelBalancing, _BmsLevelBallancing);
	Hal::ReadParameterFromEeprom16(epBmsLevelStartBalancing, _BmsLevelBallancingStart);
	Hal::ReadParameterFromEeprom16(epBmsLevelCharged, _BmsLevelCharged);

	/*Hal::ReadParameterFromEeprom8(epBmsInp1Source, (int8_t&)_Input1Source);
	Hal::ReadParameterFromEeprom8(epBmsInp2Source, (int8_t&)_Input2Source);
	Hal::ReadParameterFromEeprom8(epBmsInp3Source, (int8_t&)_Input3Source);
	Hal::ReadParameterFromEeprom8(epBmsInp4Source, (int8_t&)_Input4Source);

	Hal::ReadParameterFromEeprom8(epBmsOut1Type, (int8_t&)_Out1Type);
	Hal::ReadParameterFromEeprom8(epBmsOut2Type, (int8_t&)_Out2Type);
	Hal::ReadParameterFromEeprom8(epBmsPwm1Type, (int8_t&)_Pwm1Type);
	Hal::ReadParameterFromEeprom8(epBmsPwm2Type, (int8_t&)_Pwm2Type);*/

	_BmsProtocolHost = new ProtocolHost(1);
	_BmsReceiveMetodHost = new ReceiveMetodHost();
	_BmsSendMetodHost = new SendMetodHost();

	int8_t cellCount = 0;
	Hal::ReadParameterFromEeprom8(epBmsCellCount, cellCount);
	_ResetCellCount(cellCount);

	Hal::ReadParameterFromEeprom16(epBmsBallancingDelta, _BmsBallancingDelta);

}

BmsActiveModes _BmsNeedMode = bamNone;

int32_t _BmsTicks = 0;
void BmsKernel::Tick()
{
	/* ��� ������ �������� ������ �� ������ - �������� ��� ��������� ������������.
	 * ������� ����������� ������������ �������� �������� ���������� ������
	 * */

	uint8_t rbuf[64];
	int16_t received = Hal::UsartBms->Receive(rbuf, sizeof(rbuf));
	if (received > 0)
		for (int16_t i = 0; i < received; i++)
			OnReceiveDataFromBms(rbuf[i]);


	if (Hal::GetSpendTicks(_BmsTicks) < Hal::GetTicksInSecond() * 1)
		return;

	bool okCellsConnection = true;
	for (uint8_t i = 0; i < _CellCount; i++)
	{
		if (! (_BmsConnectState[i/8] & (1 << (i % 8))))
		{
			okCellsConnection = false;
			break;
		}
	}

	if (!okCellsConnection)
	{
		//!_BmsActiveMode = bamErrLostConnection;
	}

	_SendRequestForExternalModules();


	_BmsTicks = Hal::GetTickCount();


}

bool BmsKernel::IgnitionIsOn()
{
	return false;
}

bool BmsKernel::ChargerIsOn()
{
	return false;
}

uint8_t _SendDataBuff[32];


void BmsKernel::_SendData()
{
	uint8_t b = 0;
	uint8_t buffPos = 0;

	_BmsProtocolHost->InitSendData(*_BmsSendMetodHost);

	while (_BmsProtocolHost->SendData(b))
	{
		if (sizeof(_SendDataBuff) > buffPos + 2)
			_SendDataBuff[buffPos++] = b;
	};

	if (buffPos)
		Hal::UsartBms->Send(_SendDataBuff, buffPos);
}

void BmsKernel::_TurnOnBallansing(uint8_t cellNum)
{
	_BmsSendMetodHost->InitNewMetod((uint8_t)TurnBallancingChannel);
	_BmsSendMetodHost->AddArgumentByte(_CellCount - cellNum - 1);
	_BmsSendMetodHost->AddArgumentBool(true);

	_SendData();

}

void BmsKernel::_SendRequestForExternalModules()
{

	_BmsSendMetodHost->InitNewMetod((uint8_t)CellState);
	_BmsSendMetodHost->AddArgumentByte(0xff); //element.No
	_BmsSendMetodHost->AddArgumentByte(0); //element.State
	_BmsSendMetodHost->AddArgumentShort(0); //element.AdcValue
	_BmsSendMetodHost->AddArgumentShort(0); //element.AdcThermo
	_BmsSendMetodHost->AddArgumentShort(0); //element.AdcThermoBallancer
	_BmsSendMetodHost->AddArgumentByte(0); //element.Version

	_SendData();

}

void BmsKernel::_SendCalibrateForExternalModules(uint8_t cellNum, int16_t cellVoltage)
{
	_BmsSendMetodHost->InitNewMetod((uint8_t)CellCalibrate);
	_BmsSendMetodHost->AddArgumentByte(cellNum);
	_BmsSendMetodHost->AddArgumentShort(cellVoltage);
	_SendData();
}

void BmsKernel::CalibrateVoltage(uint8_t cellNum, int16_t cellVoltage)
{
	_SendCalibrateForExternalModules(_CellCount - cellNum - 1, cellVoltage);
}

void BmsKernel::OnReceiveDataFromBms(uint8_t data)
{
	unsigned char dataLength = 0;
	unsigned char* rData = 0;

	rData = _BmsProtocolHost->ReceiveData(data, dataLength);
	if (rData == 0)
		return;

	_BmsReceiveMetodHost->InitNewMetod(rData, dataLength);

	unsigned char mNum = _BmsReceiveMetodHost->GetMetodNumber();

	switch (mNum)
	{
	case CellState:
		_TryReceiveCellState();
		break;
	default:
		break;
	}


}

BmsActiveModes BmsKernel::_GetActiveMode()
{
	return _BmsActiveMode;
}

BmsActiveModes BmsKernel::GetActiveMode()
{
	return _GetActiveMode();
}


int32_t _AveragedCellsVoltage = 0;
int8_t _AveragedCellsVoltageFactor = 8;

static ReceivedCellStateCallBack _ReceivedCellStateCallBack = 0;
void BmsKernel::SetReceivedCellStateCallBack(ReceivedCellStateCallBack receivedCellState)
{
	_ReceivedCellStateCallBack = receivedCellState;
}

static bool _TransmitCellState = false;
void BmsKernel::TransmitCellState(bool value)
{
	_TransmitCellState = value;
}

bool BmsKernel::TransmitCellState()
{
	return _TransmitCellState;
}

void BmsKernel::_OnReceivedCellState(CellInfo& cellInfo)
{
	if (_AveragedCellsVoltage == 0) //On starting system
		_AveragedCellsVoltage = cellInfo.Voltage << _AveragedCellsVoltageFactor;

	_AveragedCellsVoltage += cellInfo.Voltage - (_AveragedCellsVoltage >> _AveragedCellsVoltageFactor);

	_BmsConnectState[cellInfo.Num / 8] |= (1 << (cellInfo.Num % 8));

	switch (_GetActiveMode())
	{
	case bamCharging:
		if (_DoBallancing(cellInfo.Num, cellInfo.Voltage))
			cellInfo.State = 11;
		break;
	case bamDischarging:
		break;
	default:
		break;
	}

	if (_ReceivedCellStateCallBack)
		_ReceivedCellStateCallBack(cellInfo);

}

bool BmsKernel::_DoBallancing(uint8_t cellNum, int16_t cellVoltage)
{
	int16_t averagedCellsVoltage = _AveragedCellsVoltage >> _AveragedCellsVoltageFactor;

	if (cellVoltage < averagedCellsVoltage + _BmsBallancingDelta)
		return false;

	if (cellVoltage < _BmsLevelBallancingStart)
		return false;

	_TurnOnBallansing(cellNum);
	return true;
}

void BmsKernel::_TryReceiveCellState()
{
	CellInfo cellInfo;
	if (!_BmsReceiveMetodHost->GetArgumentByte(0, cellInfo.Num))
		return;

	if (!_BmsReceiveMetodHost->GetArgumentByte(1, cellInfo.State))
		return;

	if (!_BmsReceiveMetodHost->GetArgumentShort(2, cellInfo.Voltage))
		return;

	if (!_BmsReceiveMetodHost->GetArgumentShort(3, cellInfo.Thermo))
		return;

	if (!_BmsReceiveMetodHost->GetArgumentShort(4, cellInfo.ThermoBal))
		return;

	if (!_BmsReceiveMetodHost->GetArgumentByte(5, cellInfo.Ver))
		return;

	_OnReceivedCellState(cellInfo);

/*    if (!receiveMetodHost.GetArgumentByte(0, ref receiveElementBuff.No))
        return null;

    byte b = 0;
    if (!receiveMetodHost.GetArgumentByte(1, ref b))
        return null;
    receiveElementBuff.State = (CellStates)b;

    if (!receiveMetodHost.GetArgumentShort(2, ref receiveElementBuff.AdcValue))
        return null;
    if (!receiveMetodHost.GetArgumentShort(3, ref receiveElementBuff.AdcThermo))
        return null;

    if (receiveMetodHost.GetArgCount() > 4)
        if (!receiveMetodHost.GetArgumentShort(4, ref receiveElementBuff.AdcThermoBallancer))
            return null;

    if (receiveMetodHost.GetArgCount() > 5)
        if (!receiveMetodHost.GetArgumentByte(5, ref receiveElementBuff.Version))
            return null;


    return receiveElementBuff;*/

}

void BmsKernel::_ResetCellCount(uint8_t cellCount)
{
	if (_CellVoltages)
	{
		free(_CellVoltages);
		_CellVoltages = 0;
	}

	_CellCount = cellCount;
	if (_CellCount)
		_CellVoltages = (int16_t*)malloc(sizeof(int16_t) * _CellCount);

	_AveragedCellsVoltageFactor = 2;
	for (uint8_t cc = cellCount >> 1; cc != 0; cc >>= 1)
		_AveragedCellsVoltageFactor++;
}

void BmsKernel::SetCellCount(uint16_t cellCount)
{
	Hal::WriteParameterToEeprom16(epBmsCellCount, cellCount);
	_ResetCellCount(cellCount);
}

uint16_t BmsKernel::GetCellCount()
{
	int16_t res = 0;
	Hal::ReadParameterFromEeprom16(epBmsCellCount, res);
	return res;
}

void BmsKernel::SetBallancingDelta(int16_t delta)
{
	Hal::WriteParameterToEeprom16(epBmsBallancingDelta, delta);
	_BmsBallancingDelta = delta;
}

int16_t BmsKernel::GetBallancingDelta()
{
	int16_t res = 0;
	Hal::ReadParameterFromEeprom16(epBmsBallancingDelta, res);
	return res;
}


int16_t BmsKernel::GetCellVoltage(int8_t cellNum)
{
	if (cellNum >= _CellCount)
		return -1;
	return _CellVoltages[cellNum];
}

void BmsKernel::SetVoltageLevel(VoltageLevels level, int16_t value)
{
	switch (level)
	{
	case vlMinVoltage:
		Hal::WriteParameterToEeprom16(epBmsLevelMin, value);
		_BmsLevelMin = value;
		break;
	case vlMaxVoltage:
		Hal::WriteParameterToEeprom16(epBmsLevelMax, value);
		_BmsLevelMax = value;
		break;
	case vlStartBalancing:
		Hal::WriteParameterToEeprom16(epBmsLevelStartBalancing, value);
		_BmsLevelBallancingStart = value;
		break;
	case vlBalancing:
		Hal::WriteParameterToEeprom16(epBmsLevelBalancing, value);
		_BmsLevelBallancing = value;
		break;
	case vlCharged:
		Hal::WriteParameterToEeprom16(epBmsLevelCharged, value);
		_BmsLevelCharged = value;
		break;
	}
}

int16_t BmsKernel::GetVoltageLevel(VoltageLevels level)
{
	int16_t res = 0;
	switch (level)
	{
	case vlMinVoltage:
		Hal::ReadParameterFromEeprom16(epBmsLevelMin, res);
		break;
	case vlMaxVoltage:
		Hal::ReadParameterFromEeprom16(epBmsLevelMax, res);
		break;
	case vlBalansing:
		Hal::ReadParameterFromEeprom16(epBmsLevelBalansing, res);
		break;
	case vlCharged:
		Hal::ReadParameterFromEeprom16(epBmsLevelCharged, res);
		break;
	}

	return res;
}

InputSources BmsKernel::GetInputSource(int8_t sNum /*1-4*/)
{
	int8_t res = 0;
	switch (sNum)
	{
	case 1:
		Hal::ReadParameterFromEeprom8(epBmsInp1Source, res);
		break;
	case 2:
		Hal::ReadParameterFromEeprom8(epBmsInp2Source, res);
		break;
	case 3:
		Hal::ReadParameterFromEeprom8(epBmsInp3Source, res);
		break;
	case 4:
		Hal::ReadParameterFromEeprom8(epBmsInp4Source, res);
		break;
	}

	return (InputSources)res;
}

void BmsKernel::SetInputSource(int8_t sNum /*1-4*/, InputSources value)
{
	switch (sNum)
	{
	case 1:
		Hal::WriteParameterToEeprom8(epBmsInp1Source, (int8_t)value);
		_Input1Source = value;
		break;
	case 2:
		Hal::WriteParameterToEeprom8(epBmsInp2Source, (int8_t)value);
		_Input2Source = value;
		break;
	case 3:
		Hal::WriteParameterToEeprom8(epBmsInp3Source, (int8_t)value);
		_Input3Source = value;
		break;
	case 4:
		Hal::WriteParameterToEeprom8(epBmsInp4Source, (int8_t)value);
		_Input4Source = value;
		break;
	}
}

OutTypes BmsKernel::GetOutputType(int8_t oNum /*1-2*/)
{
	int8_t res = 0;
	switch (oNum)
	{
	case 1:
		Hal::ReadParameterFromEeprom8(epBmsOut1Type, res);
		break;
	case 2:
		Hal::ReadParameterFromEeprom8(epBmsOut2Type, res);
		break;
	}

	return (OutTypes)res;
}

void BmsKernel::SetOutputType(int8_t oNum /*1-2*/, OutTypes value)
{
	switch (oNum)
	{
	case 1:
		Hal::WriteParameterToEeprom8(epBmsOut1Type, (int8_t)value);
		_Out1Type = value;
		break;
	case 2:
		Hal::WriteParameterToEeprom8(epBmsOut2Type, (int8_t)value);
		_Out2Type = value;
		break;
	}
}

