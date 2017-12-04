#pragma once

#include <functional>
#include "Locks.hpp"
using namespace std;

class Job
{
private:

protected:
	shared_ptr<boost::thread> m_thread;
	function<void()> m_function;

public:
	Job();
	Job(function<void()> function);

	void run();
	void runInThread();
	shared_ptr<boost::thread> getThread();
	virtual void stop();
};