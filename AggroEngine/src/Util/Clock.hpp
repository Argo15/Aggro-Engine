#pragma once

#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost::posix_time;

/**
* Clock, keeps time
*
* author: wcrane
* since: 2016-10-28
**/
class Clock
{
private:
	ptime m_startTime;
	ptime m_timerStart;

public:
	Clock();

	long getTimeSinceStartMillis();
	long getTimeSinceStartSeconds();

	long getTimerMillis();
	long getTimerSeconds();
	void resetTimer();
};