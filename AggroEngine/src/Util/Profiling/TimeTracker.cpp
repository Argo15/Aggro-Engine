#include "TimeTracker.hpp"

TimeTracker::TimeTracker(shared_ptr<ProfilingData> profilingData)
	: m_profilingData(profilingData)
{
	m_startTime = microsec_clock::local_time();
}

TimeTracker::~TimeTracker()
{
	time_duration diff = microsec_clock::local_time() - m_startTime;
	m_profilingData->addTime(diff.total_microseconds());
}