#pragma once

#include <memory>
#include "VertexBufferHandle.hpp"
using namespace std;

/**
* Loadable VertexBufferHandle that delegates to another.
*
* author: wcrane
* since: 2013-11-09
**/
class LoadableVertexBufferHandle : public VertexBufferHandle
{
private:
	shared_ptr<VertexBufferHandle> m_delegate;

public:
	LoadableVertexBufferHandle(shared_ptr<VertexBufferHandle> handle);

	unsigned int getVertexHandle() const;
	unsigned int getSizeOfVerticies() const;
	unsigned int getIndexHandle() const;
	unsigned int getSizeOfIndicies() const;
	bool hasTangents() const;
	bool isLoaded();

	void setHandle(shared_ptr<VertexBufferHandle> handle);
};