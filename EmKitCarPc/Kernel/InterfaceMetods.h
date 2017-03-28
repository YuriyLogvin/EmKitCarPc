/* 
* InterfaceMetods.h
*
* Created: 16.07.2013 18:20:44
* Author: Yuriy
*/


#ifndef __INTERFACEMETODS_H__
#define __INTERFACEMETODS_H__

typedef enum 
{
	Reset = 0,
	Ping = 1,
	VersionGet = 2,
	CellState = 3, /*�� �������� ���� � ��������*/
	BmsState = 4,
	SetCellCount = 5,
	SetProgrammingMode = 6,
	CellCalibrate = 7,
	SetIsLastModule = 8,
	SetParameterShort = 9,
	GetParameterShort = 10,
	UpdateParameterShort = 11, /*��� ���������� ��������� �� �������� ������*/
	CellStateFromBms = 12, /*�� �������� � �������� �-��*/
	UpdateParameterShortFromBms = 13, /*�� �������� � �������� �-��*/
	GetChargeSignalSource = 14,
	SetChargeSignalSource = 15,
	GetPowerSignalSource = 16,
	SetPowerSignalSource = 17,
	GetExt1SignalOnSource = 18,
	SetExt1SignalOnSource = 19,
	SetCellVoltage = 20, /*���������� ������� �������� �� ������ (����������)*/
	SetDisableBallancing = 21, /*����������/����� ������ �� ������������*/
	TurnBallancingChannel = 24, /*�������� ��� ��������� ����� ������������ ������������� (��� ���������� SetDisableBallancing)*/
	ResetEeprom = 36
} InterfaceMetods;

#endif //__INTERFACEMETODS_H__
