#pragma once

class LogData;

/*
	日志信息处理基类
*/
class LogHandler
{
public:
	virtual void Handle(const LogData& logData) = 0;
	virtual ~LogHandler(void) = 0;
};