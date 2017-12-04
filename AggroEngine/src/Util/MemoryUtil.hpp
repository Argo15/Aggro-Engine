#pragma once

#include <memory>
using namespace std;

class mem
{
public:
	template<class T> static shared_ptr<T> shared_array(T *data)
	{
		return shared_ptr<T>(data, [](T* p) {delete[] p; });
	}
};