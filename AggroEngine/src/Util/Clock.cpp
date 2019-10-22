#include "Clock.hpp"

Clock::Clock()
{
	m_startTime = microsec_clock::universal_time();
	m_timerStart = m_startTime;
}

long Clock::getTimeSinceStartMicro()
{
	time_duration diff = microsec_clock::universal_time() - m_startTime;
	return diff.total_microseconds();
}

long Clock::getTimeSinceStartMillis()
{
	time_duration diff = microsec_clock::universal_time() - m_startTime;
	return diff.total_milliseconds();
}

long Clock::getTimeSinceStartSeconds()
{
	time_duration diff = microsec_clock::universal_time() - m_startTime;
	return diff.total_seconds();
}

long Clock::getTimerMicro()
{
	time_duration diff = microsec_clock::universal_time() - m_timerStart;
	return diff.total_microseconds();
}

long Clock::getTimerMillis()
{
	time_duration diff = microsec_clock::universal_time() - m_timerStart;
	return diff.total_milliseconds();
}

long Clock::getTimerSeconds()
{
	time_duration diff = microsec_clock::universal_time() - m_timerStart;
	return diff.total_seconds();
}

void Clock::resetTimer()
{
	m_timerStart = microsec_clock::universal_time();
}