#include "Core\Log\LogManager.h"
#include "Core\Log\LogHandler.h"

/* LogData */
LogData::LogData(const String &logStr, LogData::LogType logType) : m_logStr(logStr), m_logType(logType)
{
	m_logTime = Time::Now();
}

void LogData::ReUse(const String& logStr, LogType logType)
{
	m_logStr = String(logStr);
	m_logType = logType;
	m_logTime = Time::Now();
}

/* LogManager */

void LogManager::Log(const String& logInfo, LogData::LogType logType)
{
	const LogData* newLogData;
	if (m_logBuffer.size() == LOG_BUFFER_MAX_SIZE)
	{
		LogData *logData = const_cast<LogData*>(m_logBuffer.front());
		m_logBuffer.pop();
		logData->ReUse(logInfo, logType);

		newLogData = logData;
	}
	else
		newLogData = new LogData(logInfo, logType);

	//Notify Handler
	//TODO : should use asset in debug?
	if (m_logHandler != nullptr)
		m_logHandler->Handle(*newLogData);
}

void LogManager::OnDestroy(void)
{
	//TODO : File Flush ?
	delete m_logHandler;
}