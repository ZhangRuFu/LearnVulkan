#pragma once

#include <queue>
#include "Core\Misc\Singleton.h"
#include "Core\Container\String.h"
#include "Core\Time\Time.h"

class LogHandler;

/*
	LogData
	日志调试信息
*/
class LogData
{
public:
	enum class LogType { Log, Warning, Error };

private:
	LogType m_logType;
	String m_logStr;
	Time m_logTime;

public:
	LogData(const String& logStr, LogType logType);

	void ReUse(const String& logStr, LogType logType);

	LogType GetLogType(void) const { return m_logType; }
	const String& GetLogString(void) const { return m_logStr; }
	const Time& GetLogTime(void) const { return m_logTime; }

};

/*
	LogManager
	对外的Debug接口调试信息将缓存在LogManager
	通过LogHandler进行调试信息的输出
*/
class LogManager : public Singleton<LogManager>
{
private:
	std::queue<const LogData*> m_logBuffer;
	const int LOG_BUFFER_MAX_SIZE = 100;

	//暂时使用一个
	LogHandler *m_logHandler = nullptr;

public:
	int GetLogBufferCount(void) const { return m_logBuffer.size(); }
	void Log(const String& logInfo, LogData::LogType logType);

	void SetLogHandler(LogHandler *logHandler) { m_logHandler = logHandler; }

public:
	virtual void OnInit(void) {}
	virtual void OnDestroy(void);
};