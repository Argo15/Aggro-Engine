#pragma once


#include <memory>
#include <iostream>
#include <queue>
#include "Locks.hpp"
#include "Listener.hpp"
#include "ProfilingData.hpp"
#include "TimeTracker.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/unordered_map.hpp>
using namespace boost::posix_time;

/**
* Holds performance information about the application such as fps and memory usage
*
* author: wcrane
* since: 2013-11-03
**/
class PerfStats : public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	ptime m_startTime;
	std::queue<long> m_frameTimestamps;
	ptime m_lastFPSTick;
	int m_fpsTicksTracked;
	std::deque<int> m_fpsTicks;
	Listener<std::deque<int> *> m_fpsListeners;
	boost::unordered_map<string, long> m_bytesAllocated;
	Listener<boost::unordered_map<string, long> *> m_memoryListeners;
	bool m_alertMemoryListeners;
	bool m_profilerEnabled;
	boost::unordered_map<string, shared_ptr<ProfilingData>> m_profilingData;
	Listener<boost::unordered_map<string, shared_ptr<ProfilingData>> *> m_profilingListeners;

public:
	PerfStats();

	void recordFrame();
	int getFPS();
	std::deque<int> getFPSTicks();
	void addFPSListener(function<void(std::deque<int> *)> func);

	void addBytesAllocated(string name, long bytes);
	boost::unordered_map<string, long> &getBytesAllocated();
	void addMemoryListener(function<void(boost::unordered_map<string, long> *)> func);

	shared_ptr<TimeTracker> trackTime(string name);
	boost::unordered_map<string, shared_ptr<ProfilingData>> &getProfilingData();
	void addProfilingListener(function<void(boost::unordered_map<string, shared_ptr<ProfilingData>> *)> func);

	static PerfStats& instance()
	{
		static PerfStats INSTANCE;
		return INSTANCE;
	}
};