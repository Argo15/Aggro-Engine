#include "BufferSyncContext.hpp"

BufferSyncContext::BufferSyncContext()
{
}

void BufferSyncContext::addSync(GLuint buffHandle, GLsync syncObj)
{
	m_buffToSync[buffHandle] = syncObj;
}

bool BufferSyncContext::checkAndClearSync(GLuint buffHandle)
{
	if (m_buffToSync.find(buffHandle) == m_buffToSync.end())
	{
		return true;
	}

	GLsync sync = m_buffToSync[buffHandle];
	GLenum signalResponse = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
	if (signalResponse == GL_ALREADY_SIGNALED || signalResponse == GL_CONDITION_SATISFIED)
	{
		glDeleteSync(sync);
		m_buffToSync.erase(buffHandle);
		return true;
	}

	return false;
}