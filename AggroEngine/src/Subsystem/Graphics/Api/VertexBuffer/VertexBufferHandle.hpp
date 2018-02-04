#pragma once

/**
 * Provides a handle to a vertex data stored on the GPU
 *
 * author: wcrane
 * since: 2013-11-09
**/
class VertexBufferHandle
{
public:
	virtual unsigned int getVertexHandle() const = 0;
	virtual unsigned int getSizeOfVerticies() const = 0;
	virtual unsigned int getIndexHandle() const = 0;
	virtual unsigned int getSizeOfIndicies() const = 0;
	virtual bool hasTangents() const = 0;
	virtual bool isLoaded() = 0;
};