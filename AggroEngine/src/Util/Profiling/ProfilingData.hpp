#pragma once

#include <string>
#include <queue>
using namespace std;

/**
* Contains time spent in a block of code in microseconds
*
* author: wcrane
* since: 2018-09-03
**/
class ProfilingData
{
private:
	string m_name;
	long m_maxTime;
	long m_sumTime;
	deque<int> m_allTimes;
	long m_frameTime;

public:
	ProfilingData();

	void addTime(long time);
	void endFrame();

	long getMaxTime();
	double getAverageTime();
};