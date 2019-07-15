#include "Platform\Win32\WindowsConsoleLogHandler.h"
#include "Core\Log\LogManager.h"

WindowsConsoleLogHandler::WindowsConsoleLogHandler()
{
	AllocConsole();

	m_consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

void WindowsConsoleLogHandler::Handle(const LogData& logData)
{
	//No A will use wchar_t version
	WriteConsole(m_consoleOutput, logData.GetLogString().c_str(), logData.GetLogString().length(), nullptr, nullptr);
}

WindowsConsoleLogHandler::~WindowsConsoleLogHandler(void)
{
	FreeConsole();
}
