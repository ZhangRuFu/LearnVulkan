#pragma once

/*
	Time
	时间数据封装
	精度：ms
*/

class Time
{
private:
	long long m_timeSinceEpoch;

public:
	Time(void) : m_timeSinceEpoch(0) { }

public:
	static Time Now(void);
};