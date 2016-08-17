#pragma once

/**
 * @Deprecated - Use Mesh Instead
 *
 * Provides a handle to a vertex data stored on the GPU
 *
 * author: wcrane
 * since: 2013-11-09
**/
class VertexBufferHandle
{
private:
	unsigned int m_nVertexHandle;
	unsigned int m_nSizeOfVerticies;
	unsigned int m_nIndexHandle;
	unsigned int m_nSizeOfIndicies;

public:
	VertexBufferHandle(unsigned int nVertexHandle, unsigned int nSizeOfVerticies, unsigned int nIndexHandle, unsigned int nSizeOfIndicies);
	
	unsigned int getVertexHandle() const;
	unsigned int getSizeOfVerticies() const;
	unsigned int getIndexHandle() const;
	unsigned int getSizeOfIndicies() const;
};