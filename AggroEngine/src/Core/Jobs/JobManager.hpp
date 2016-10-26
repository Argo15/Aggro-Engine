#pragma once

#include "Job.hpp"

class JobManager
{
private:

public:
	JobManager();

	void runJob(Job *job);
};