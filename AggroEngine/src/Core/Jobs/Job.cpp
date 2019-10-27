#include "Job.hpp"
#include "PerfStats.hpp"
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
		auto tracker = PerfStats::instance().trackTime("Job::run");
		m_function();
		auto tracker1 = PerfStats::instance().trackTime("Job::callback");
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