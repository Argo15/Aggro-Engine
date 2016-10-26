#include "JobManager.hpp"

JobManager::JobManager()
{
}


void JobManager::runJob(Job *job)
{
	job->run();
}