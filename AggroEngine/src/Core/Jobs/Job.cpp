#include "Job.hpp"

Job::Job(function<void()> function)
	: m_function(function)
{

}

void Job::run()
{
	m_thread = shared_ptr<boost::thread>(new boost::thread(m_function));
}

shared_ptr<boost::thread> Job::getThread()
{
	return m_thread;
}