/*
 * Shell.cpp
 *
 *  Created on: Apr 26, 2017
 *      Author: banz
 */

#include <Shell.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "BmsKernel.h"

#include "hal.h"

typedef int (*CommandMetodType)(const char*, const char* );
struct CommandMetodArray
{
	const char* CommandName;
	CommandMetodType FunctionPointer;
};

struct SShell
{
	const char* Promt;
	int8_t Num;
	CommandMetodArray * ActiveCommands;
};

void _SendCommandRespond(const char* resp)
{
	while (!Hal::UsartWiFi->Send((uint8_t*)resp, strlen(resp)));
}

SShell _Shell;
SShell _ShellPrev;

int _CommandLevels(const char* params, VoltageLevels lvl, const char* commandName)
{
	int16_t level;
	if (params)
	{
		level = atoi(params);
		BmsKernel::SetVoltageLevel(lvl, level);
	}

	level = BmsKernel::GetVoltageLevel(lvl);
	char buff[32];
	sprintf(buff, "\t%s = %i\r\n", commandName, level);
	_SendCommandRespond(buff);

	return 0;
}

int CommandLevelsMax(const char* params, const char* commandName)
{
	return _CommandLevels(params, vlMaxVoltage, commandName);
}

int CommandLevelsMin(const char* params, const char* commandName)
{
	return _CommandLevels(params, vlMinVoltage, commandName);
}

int CommandLevelsBal(const char* params, const char* commandName)
{
	return _CommandLevels(params, vlBalansing, commandName);
}

int CommandLevelsCrgd(const char* params, const char* commandName)
{
	return _CommandLevels(params, vlCharged, commandName);
}

int CommandLevelsAll(const char* params, const char* commandName);

const char* CommandsLevelsPromt = "levels>";
static CommandMetodArray CommandsLevels[]  =
{
	{"max", CommandLevelsMax},
	{"min", CommandLevelsMin},
	{"bal", CommandLevelsBal},
	{"chrgd", CommandLevelsCrgd},
	{"all", CommandLevelsAll},
};

int CommandLevelsAll(const char* params, const char* commandName)
{
	for (int8_t i = 0; i+1 < sizeof(CommandsLevels)/sizeof(CommandMetodArray); i++)
	{
		CommandsLevels[i].FunctionPointer(0, CommandsLevels[i].CommandName);
	}
	return 0;
}


int CommandLevels(const char* params, const char* commandName)
{
	_ShellPrev = _Shell;

	_Shell.Num = sizeof(CommandsLevels)/sizeof(CommandMetodArray);
	_Shell.ActiveCommands = CommandsLevels;
	_Shell.Promt = CommandsLevelsPromt;

	Shell::ProcessCommand("");
}

int CommandConfigCells(const char* params, const char* commandName)
{
	int16_t num;
	if (params)
	{
		num = atoi(params);
		BmsKernel::SetCellCount(num);
	}

	num = BmsKernel::GetCellCount();
	char buff[32];
	sprintf(buff, "\t%s = %i\r\n", commandName, num);
	_SendCommandRespond(buff);

	return 0;
}

const char* CommandsConfigPromt = "config>";
static CommandMetodArray CommandsConfig[]  =
{
	{"cells", CommandConfigCells},
};

int CommandConfig(const char* params, const char* commandName)
{
	_ShellPrev = _Shell;

	_Shell.Num = sizeof(CommandsConfig)/sizeof(CommandMetodArray);
	_Shell.ActiveCommands = CommandsConfig;
	_Shell.Promt = CommandsConfigPromt;

	Shell::ProcessCommand("");
}


const char* CommandsRootPromt = ">";
static CommandMetodArray CommandsRoot[]  =
{
	{"levels", CommandLevels},
	{"config", CommandConfig},
};


void Shell::Init()
{
	_Shell.Num = sizeof(CommandsRoot)/sizeof(CommandMetodArray);
	_Shell.ActiveCommands = CommandsRoot;
	_Shell.Promt = CommandsRootPromt;
}

void Shell::ProcessCommand(char* command)
{
	char* param = 0;
	uint8_t commandLen;

	if (command[0] == 0)
	{
		_SendCommandRespond(_Shell.Promt);
		return;
	}

	if (strcmp(command, "?") == 0)
	{
		for (int8_t i = 0; i < _Shell.Num; i++)
		{
			_SendCommandRespond("\t");
			_SendCommandRespond(_Shell.ActiveCommands[i].CommandName);
			_SendCommandRespond("\r\n");
		}
		_SendCommandRespond(_Shell.Promt);
		return;
	}

	if (strcmp(command, "..") == 0)
	{
		_Shell = _ShellPrev;
		_SendCommandRespond("\r\n");
		_SendCommandRespond(_Shell.Promt);
		return;
	}


	param = strchr(command, ' ');
	if (param)
	{
		param[0] = 0;
		param++;
	}

	for (int8_t i = 0; i < _Shell.Num; i++)
	{
		if (strcmp(_Shell.ActiveCommands[i].CommandName, command) != 0)
			continue;

		if (!_Shell.ActiveCommands[i].FunctionPointer)
			break;

		_Shell.ActiveCommands[i].FunctionPointer(param, _Shell.ActiveCommands[i].CommandName);

		break;
	}
}

