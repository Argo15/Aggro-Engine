#include "DefaultVertexBufferHandle.hpp"

DefaultVertexBufferHandle::DefaultVertexBufferHandle()
	: m_loaded(false)
{

}

DefaultVertexBufferHandle::DefaultVertexBufferHandle(unsigned int nVertexHandle, unsigned int nSizeOfVerticies, unsigned int nIndexHandle, unsigned int nSizeOfIndicies)
	: m_nVertexHandle(nVertexHandle)
	, m_nSizeOfVerticies(nSizeOfVerticies)
	, m_nIndexHandle(nIndexHandle)
	, m_nSizeOfIndicies(nSizeOfIndicies)
	, m_loaded(true)
{
}

unsigned int DefaultVertexBufferHandle::getVertexHandle() const
{
	return m_nVertexHandle;
}

unsigned int DefaultVertexBufferHandle::getSizeOfVerticies() const
{
	return m_nSizeOfVerticies;
}

unsigned int DefaultVertexBufferHandle::getIndexHandle() const
{
	return m_nIndexHandle;
}

unsigned int DefaultVertexBufferHandle::getSizeOfIndicies() const
{
	return m_nSizeOfIndicies;
}

bool DefaultVertexBufferHandle::isLoaded()
{
	return m_loaded;
}