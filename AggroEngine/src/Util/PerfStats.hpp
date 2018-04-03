#pragma once


#include <memory>
#include <iostream>
#include <queue>
#include "Locks.hpp"
#include "Listener.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost::posix_time;

/**
* Holds performance information about the application such as fps and memory usage
*
* author: wcrane
* since: 2013-11-03
**/
class PerfStats : public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	ptime m_startTime;
	std::queue<long> m_frameTimestamps;
	ptime m_lastFPSTick;
	int m_fpsTicksTracked;
	std::deque<int> m_fpsTicks;
	Listener<std::deque<int> *> m_fpsListeners;

public:
	PerfStats();

	void recordFrame();
	int getFPS();
	std::deque<int> getFPSTicks();
	void addFPSListener(function<void(std::deque<int> *)> func);

	static PerfStats& instance()
	{
		static PerfStats INSTANCE;
		return INSTANCE;
	}
};