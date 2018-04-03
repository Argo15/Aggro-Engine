#include "PerfStats.hpp"
#include "Config.hpp"

PerfStats::PerfStats()
	: m_frameTimestamps()
{
	m_startTime = microsec_clock::local_time();
	m_lastFPSTick = m_startTime;
	m_fpsTicksTracked = -1;
	m_fpsTicksTracked = Config::instance().getProperties().getIntProperty("engine.performance.fps_ticks_tracked", 240);
}

void PerfStats::recordFrame()
{
	boost::lock_guard<PerfStats> guard(*this);


	ptime curTime = microsec_clock::local_time();
	long timestamp = (curTime - m_startTime).total_microseconds();
	m_frameTimestamps.push(timestamp);

	long pastSecondTS = timestamp - 1000000;
	while (m_frameTimestamps.size() > 0 && m_frameTimestamps.front() <= pastSecondTS)
	{
		m_frameTimestamps.pop();
	}

	long timeSinceFPSTick = (curTime - m_lastFPSTick).total_milliseconds();
	if (timeSinceFPSTick >= 100)
	{
		m_lastFPSTick = curTime;
		m_fpsTicks.push_back(m_frameTimestamps.size());
		if (m_fpsTicks.size() > m_fpsTicksTracked)
		{
			m_fpsTicks.pop_front();
		}
		m_fpsListeners.notify(&m_fpsTicks);
	}
}

int PerfStats::getFPS()
{
	boost::lock_guard<PerfStats> guard(*this);
	return m_frameTimestamps.size();
}

std::deque<int> PerfStats::getFPSTicks()
{
	boost::lock_guard<PerfStats> guard(*this);
	return m_fpsTicks;
}

void PerfStats::addFPSListener(function<void(std::deque<int> *)> func)
{
	boost::lock_guard<PerfStats> guard(*this);
	m_fpsListeners.add(func);
}