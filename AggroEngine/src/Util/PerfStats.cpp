#include "PerfStats.hpp"

PerfStats::PerfStats()
	: m_frameTimestamps()
{
	m_startTime = microsec_clock::local_time();
}

void PerfStats::recordFrame()
{
	boost::lock_guard<PerfStats> guard(*this);
	long timestamp = (microsec_clock::local_time() - m_startTime).total_microseconds();
	m_frameTimestamps.push(timestamp);

	long pastSecondTS = timestamp - 1000000;
	while (m_frameTimestamps.size() > 0 && m_frameTimestamps.front() <= pastSecondTS)
	{
		m_frameTimestamps.pop();
	}
}

int PerfStats::getFPS()
{
	boost::lock_guard<PerfStats> guard(*this);
	return m_frameTimestamps.size();
}