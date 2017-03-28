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
	CellState = 3, /*ќт дочерних плат к головной*/
	BmsState = 4,
	SetCellCount = 5,
	SetProgrammingMode = 6,
	CellCalibrate = 7,
	SetIsLastModule = 8,
	SetParameterShort = 9,
	GetParameterShort = 10,
	UpdateParameterShort = 11, /*дл€ обновлени€ параметра на дочерних платах*/
	CellStateFromBms = 12, /*ќт головной к внешнему у-ву*/
	UpdateParameterShortFromBms = 13, /*ќт головной к внешнему у-ву*/
	GetChargeSignalSource = 14,
	SetChargeSignalSource = 15,
	GetPowerSignalSource = 16,
	SetPowerSignalSource = 17,
	GetExt1SignalOnSource = 18,
	SetExt1SignalOnSource = 19,
	SetCellVoltage = 20, /*установить текущее значение на €чейке (калибровка)*/
	SetDisableBallancing = 21, /*установить/сн€ть запрет на балансировку*/
	TurnBallancingChannel = 24, /*¬ключить или выключить канал балансировки принудительно (при включенном SetDisableBallancing)*/
	ResetEeprom = 36
} InterfaceMetods;

#endif //__INTERFACEMETODS_H__
