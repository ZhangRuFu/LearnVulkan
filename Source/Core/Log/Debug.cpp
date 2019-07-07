#include "Core\Container\String.h"
#include "Core\Log\Debug.h"
#include "Core\Log\LogManager.h"


void Debug::Log(const String& logStr)
{
	LogManager::Instance()->Log(logStr, LogData::LogType::Log);
}

void Debug::Warning(const String& warStr)
{
	LogManager::Instance()->Log(warStr, LogData::LogType::Warning);
}

void Debug::Error(const String& errStr)
{
	LogManager::Instance()->Log(errStr, LogData::LogType::Error);
}