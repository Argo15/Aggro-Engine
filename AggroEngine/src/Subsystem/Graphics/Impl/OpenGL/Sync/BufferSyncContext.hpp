#pragma once

#include "Graphics.hpp"
#include <boost/unordered_map.hpp>

class BufferSyncContext
{
private:
	boost::unordered_map<GLuint, GLsync> m_buffToSync;
public:
	BufferSyncContext();

	void addSync(GLuint buffHandle, GLsync syncObj);
	// true if sync is done or there is no sync. Deletes sync object if exists and is synced.
	bool checkAndClearSync(GLuint buffHandle);
};