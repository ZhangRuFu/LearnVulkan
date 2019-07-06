#include <chrono>
#include "Core\Time\Time.h"

Time Time::Now(void)
{
	Time time;
	time.m_timeSinceEpoch = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
	return time;
}