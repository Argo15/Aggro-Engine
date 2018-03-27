#pragma once


#include <memory>
#include <iostream>
#include <queue>
#include "Locks.hpp"
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

public:
	PerfStats();

	void recordFrame();
	int getFPS();
};

static std::shared_ptr<PerfStats> gPerfStats = std::shared_ptr<PerfStats>(new PerfStats());