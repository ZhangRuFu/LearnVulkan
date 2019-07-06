#pragma once
#include <Windows.h>
#include "Core\Log\LogHandler.h"


/*
	Windows Console Log Out
*/

class WindowsConsoleLogHandler : public LogHandler
{
private:
	HANDLE m_consoleOutput;

public:
	WindowsConsoleLogHandler(void);

	virtual void Handle(const LogData& logData);
	virtual ~WindowsConsoleLogHandler(void);
};