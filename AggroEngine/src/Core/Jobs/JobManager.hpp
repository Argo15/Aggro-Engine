#pragma once

#include <queue>
#include "Job.hpp"

enum JobPriority
{
	HIGH,
	MEDIUM,
	LOW
};

class JobManager : public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	int m_numThreads;
	vector<queue<shared_ptr<Job>>> m_jobQueue;
	vector<shared_ptr<Job>> m_runningJobs;
	queue<shared_ptr<Job>> m_graphicsJobs;

public:
	JobManager();
	~JobManager();

	void run(Job *job);
	void add(shared_ptr<Job> job, JobPriority priority = MEDIUM);

	void addGraphicsJob(shared_ptr<Job> job);
	shared_ptr<Job> nextGraphicsJob();

	void tick();
};