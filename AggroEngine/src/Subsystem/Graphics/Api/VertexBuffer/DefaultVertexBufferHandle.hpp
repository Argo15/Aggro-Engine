#pragma once

#include "VertexBufferHandle.hpp"

class DefaultVertexBufferHandle : public VertexBufferHandle
{
private:
	unsigned int m_nVertexHandle;
	unsigned int m_nSizeOfVerticies;
	unsigned int m_nIndexHandle;
	unsigned int m_nSizeOfIndicies;
	bool m_hasTangents;
	bool m_loaded;

public:
	DefaultVertexBufferHandle();
	DefaultVertexBufferHandle(unsigned int nVertexHandle, unsigned int nSizeOfVerticies, unsigned int nIndexHandle, unsigned int nSizeOfIndicies, bool hasTangents);

	unsigned int getVertexHandle() const;
	unsigned int getSizeOfVerticies() const;
	unsigned int getIndexHandle() const;
	unsigned int getSizeOfIndicies() const;
	bool hasTangents() const;
	bool isLoaded();
};