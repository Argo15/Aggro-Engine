#pragma once

#include <memory>
#include "PerfStats.hpp"
using namespace std;

class mem
{
public:
	template<class T> static shared_ptr<T> shared_array(
		T *data, 
		int numObjects, 
		string ns, 
		boost::optional<function<void(T*)>> deleteFunc = boost::optional<function<void(T*)>>()
	)
	{
		int numBytes = numObjects * sizeof(T);
		PerfStats::instance().addBytesAllocated(ns, numBytes);
		return shared_ptr<T>(data, [ns, numBytes, deleteFunc](T* p) {
			PerfStats::instance().addBytesAllocated(ns, -numBytes); 
			if (deleteFunc)
			{
				(*deleteFunc)(p);
			}
			else
			{
				delete[] p;
			}
		});
	}
};