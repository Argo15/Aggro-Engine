#include "TickingJob.hpp"

static const long MICROSECONDS_PER_SECOND = 1000000;

TickingJob::TickingJob(int updateFps)
	: m_updateFps(updateFps)
	, m_clock(new Clock())
	, m_killed(false)
{
	m_function = [this]() {
		while (!m_killed)
		{
			long timePassedMicro = m_clock->getTimerMicro();
			long timeRemainingMicro = (MICROSECONDS_PER_SECOND / m_updateFps) - timePassedMicro;
			// Allow it to go 0.5ms early, better to have a faster fps than slower
			// High framerates, say 300 fps tends to clamp to either 330 fps or 280 fps on my cpu
			// Possibly due to m_clock->getTimerMicro() overhead?
			if (timeRemainingMicro <= 500) 
			{
				m_clock->resetTimer();
				tick(MICROSECONDS_PER_SECOND / timePassedMicro);
			}
			else if (timeRemainingMicro > 500)
			{
				boost::this_thread::sleep_for(boost::chrono::microseconds(timeRemainingMicro - 500));
			}
		}
		return;
	};
}

void TickingJob::stop()
{
	m_killed = true;
	if (m_thread)
	{
		m_thread->join();
	}
}