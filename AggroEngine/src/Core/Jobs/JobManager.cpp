#include "JobManager.hpp"
#include "Config.hpp"

JobManager::JobManager()
	: m_graphicsJobs()
{
	m_numThreads = gConfig->getProperties().getIntProperty("engine.worker_threads");
	m_jobQueue = vector<queue<shared_ptr<Job>>>(3, queue<shared_ptr<Job>>());
}

JobManager::~JobManager()
{
	lock();
	unlock();
}


void JobManager::run(Job *job)
{
	boost::lock_guard<JobManager> guard(*this);
	job->run();
}

void JobManager::add(shared_ptr<Job> job, JobPriority priority)
{
	boost::lock_guard<JobManager> guard(*this);
	m_jobQueue[priority].push(job);
}

void JobManager::addGraphicsJob(shared_ptr<Job> job)
{
	boost::lock_guard<JobManager> guard(*this);
	m_graphicsJobs.push(job);
}

shared_ptr<Job> JobManager::nextGraphicsJob()
{
	boost::lock_guard<JobManager> guard(*this);
	if (m_graphicsJobs.size() == 0)
	{
		return shared_ptr<Job>();
	}
	shared_ptr<Job> job = m_graphicsJobs.front();
	m_graphicsJobs.pop();
	return job;
}

void JobManager::tick()
{
	boost::lock_guard<JobManager> guard(*this);
	// Remove any finished jobs
	for (vector<shared_ptr<Job>>::iterator it = m_runningJobs.begin(); it != m_runningJobs.end();)
	{
		if (it->get()->getThread()->timed_join(boost::posix_time::microseconds(0)))
		{
			it = m_runningJobs.erase(it);
		}
		else
		{
			++it;
		}
	}

	// Start new jobs
	while (m_runningJobs.size() < m_numThreads)
	{
		shared_ptr<Job> nextJob;
		if (m_jobQueue[JobPriority::HIGH].size() > 0)
		{
			nextJob = m_jobQueue[JobPriority::HIGH].front();
			m_jobQueue[JobPriority::HIGH].pop();
		}
		else if (m_jobQueue[JobPriority::MEDIUM].size() > 0)
		{
			nextJob = m_jobQueue[JobPriority::MEDIUM].front();
			m_jobQueue[JobPriority::MEDIUM].pop();
		}
		else if (m_jobQueue[JobPriority::LOW].size() > 0)
		{
			nextJob = m_jobQueue[JobPriority::LOW].front();
			m_jobQueue[JobPriority::LOW].pop();
		}
		else
		{
			break;
		}

		m_runningJobs.push_back(nextJob);
		nextJob->run();
	}
}