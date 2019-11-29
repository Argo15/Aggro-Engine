#include "Job.hpp"
#include <QtConcurrent>

Job::Job()
{
}

Job::Job(const function<void()> function, bool ovride)
	: m_function(function)
	, m_override(ovride)
{
}

void Job::run()
{
	m_thread = shared_ptr<boost::thread>(new boost::thread(m_function));
}

void Job::run(function<void()> callback)
{
	QtConcurrent::run([this, callback]() {
		m_function();
		callback();
	});
}

void Job::runInThread()
{
	m_function();
}

void Job::stop()
{
}