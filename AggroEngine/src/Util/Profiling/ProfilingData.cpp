#include "ProfilingData.hpp"

ProfilingData::ProfilingData()
	: m_maxTime(0)
	, m_sumTime(0)
	, m_frameTime(0)
{

}

void ProfilingData::addTime(long time)
{
	m_frameTime += time;
}

void ProfilingData::endFrame()
{
	m_maxTime = max(m_maxTime, m_frameTime);
	m_sumTime += m_frameTime;
	m_allTimes.push_back(m_frameTime);
	if (m_allTimes.size() > 1000)
	{
		m_sumTime -= m_allTimes.front();
		m_allTimes.pop_front();
	}
	m_frameTime = 0;
}

long ProfilingData::getMaxTime()
{
	return m_maxTime;
}

double ProfilingData::getAverageTime()
{
	if (m_allTimes.size() == 0)
	{
		return 0;
	}
	return ((double)m_sumTime) / m_allTimes.size();
}