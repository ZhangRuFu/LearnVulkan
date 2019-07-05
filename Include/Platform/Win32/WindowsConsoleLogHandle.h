#pragma once

#include "Core\Log\LogHandler.h"

/*
	Windows Console Log Out
*/

class WindowsConsoleLogHandle : LogHandler
{
public:
	virtual void Log(const LogData& logData);
};