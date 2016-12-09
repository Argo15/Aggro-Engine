#pragma once

#include "Job.hpp"
#include "Clock.hpp"

class EngineContext;

class TickingJob : public Job
{
private:
	shared_ptr<Clock> m_clock;
	int m_updateFps;

public:
	TickingJob(int updateFps);

	virtual void tick(int fps) = 0;
};