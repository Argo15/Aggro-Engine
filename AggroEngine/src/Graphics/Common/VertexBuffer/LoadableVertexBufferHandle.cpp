#include "LoadableVertexBufferHandle.hpp"

LoadableVertexBufferHandle::LoadableVertexBufferHandle(shared_ptr<VertexBufferHandle> handle)
	: m_delegate(handle)
{

}

unsigned int LoadableVertexBufferHandle::getVertexHandle() const
{
	return m_delegate->getVertexHandle();
}

unsigned int LoadableVertexBufferHandle::getSizeOfVerticies() const
{
	return m_delegate->getSizeOfVerticies();
}

unsigned int LoadableVertexBufferHandle::getIndexHandle() const
{
	return m_delegate->getIndexHandle();
}

unsigned int LoadableVertexBufferHandle::getSizeOfIndicies() const
{
	return m_delegate->getSizeOfIndicies();
}

bool LoadableVertexBufferHandle::isLoaded()
{
	return m_delegate->isLoaded();
}

void LoadableVertexBufferHandle::setHandle(shared_ptr<VertexBufferHandle> handle)
{
	m_delegate = handle;
}