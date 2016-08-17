#include "VertexBufferHandle.hpp"

VertexBufferHandle::VertexBufferHandle(unsigned int nVertexHandle, unsigned int nSizeOfVerticies, unsigned int nIndexHandle, unsigned int nSizeOfIndicies)
	: m_nVertexHandle(nVertexHandle)
	, m_nSizeOfVerticies(nSizeOfVerticies)
	, m_nIndexHandle(nIndexHandle)
	, m_nSizeOfIndicies(nSizeOfIndicies)
{
}
	
unsigned int VertexBufferHandle::getVertexHandle() const
{
	return m_nVertexHandle;
}

unsigned int VertexBufferHandle::getSizeOfVerticies() const
{
	return m_nSizeOfVerticies;
}

unsigned int VertexBufferHandle::getIndexHandle() const
{
	return m_nIndexHandle;
}

unsigned int VertexBufferHandle::getSizeOfIndicies() const
{
	return m_nSizeOfIndicies;
}