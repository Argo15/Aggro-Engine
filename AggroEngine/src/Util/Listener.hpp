#pragma once

#include <vector>
#include <functional>
using namespace std;

template <class T>
class Listener
{
private:
	vector<function<void(T)>> m_listeners;

public:
	template <class T> void add(function<void(T)> listener)
	{
		m_listeners.push_back(listener);
	}

	template <class T> void notify(T arg)
	{
		if (m_listeners.size() > 0)
		{
			for (auto &listener : m_listeners)
			{
				listener(arg);
			}
		}
	}
};