#pragma once

#include <memory>
#include "ProfilingData.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost::posix_time;

/**
* Keeps track of start and end time then updates a ProfilingData object when deleted. 
* Usage is a follows
* void myMethod() {
*   TimeTracker tracker(profilingData)
*   ...
* }
*
* author: wcrane
* since: 2018-09-03
**/
class TimeTracker
{
private:
	ptime m_startTime;
	shared_ptr<ProfilingData> m_profilingData;

public:
	TimeTracker(shared_ptr<ProfilingData> profilingData);
	~TimeTracker();
};