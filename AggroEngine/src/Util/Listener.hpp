#pragma once

#include <vector>
#include <functional>
#include <boost/unordered_map.hpp>
#include <boost/range/adaptor/map.hpp> 
using namespace std;

template <class T>
class Listener
{
private:
	boost::unordered_map<void *, vector<function<void(T)>>> m_hmListeners;

public:

	template <class T> void add(function<void(T)> listener)
	{
		add((void *)this, listener);
	}

	template <class T> void add(void *ns, function<void(T)> listener)
	{
		if (m_hmListeners.find(ns) == m_hmListeners.end())
		{
			// add new list
			vector<function<void(T)>> listeners;
			listeners.push_back(listener);
			m_hmListeners[ns] = listeners;
		}
		else
		{
			// append to old list
			vector<function<void(T)>> &listeners = m_hmListeners[ns];
			listeners.push_back(listener);
		}
	}

	void remove(void *ns)
	{
		if (ns != nullptr && m_hmListeners.find(ns) != m_hmListeners.end())
		{
			m_hmListeners[ns].clear();
		}
	}

	template <class T> void notify(T arg)
	{
		for (vector<function<void(T)>> listeners : (m_hmListeners | boost::adaptors::map_values))
		{
			if (listeners.size() > 0)
			{
				for (function<void(T)> &listener : listeners)
				{
					// Somewhat slow since functions are dynamically resolving lambdas
					listener(arg);
				}
			}
		}
	}
};