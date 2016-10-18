#pragma once

#include <boost/thread.hpp>
#include <memory>
using namespace std;

class Locks 
{
public:
	boost::mutex graphics;
}; 

static const shared_ptr<Locks> gLocks(new Locks());