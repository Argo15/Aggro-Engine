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
	void run(function<void()> callback);
	void runInThread();
	virtual void stop();
};