#pragma once
/**
* Helpful utils for functions
*
* author: wcrane
* since: 2016-08-30
**/

#include <vector>
#include <functional>
using namespace std;

class FunctionUtil
{
public:
	template<class T> static void runAll(vector<std::function<void(T)>> &funcs, T arg)
	{
		if (funcs.size() > 0)
		{
			for (auto &func : funcs)
			{
				func(arg);
			}
		}
	}
};