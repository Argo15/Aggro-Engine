#include "RenderData.hpp"


RenderData::RenderData()
	: m_pVertexBufferHandle()
	, m_pTextureHandle()
	, m_m4ModelMatrix(1.0)
	, m_drawMode(DrawMode::TRIANGLES)
	, m_lineWidth(1)
	, m_depthTestEnabled(true)
{
}

RenderData::RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, shared_ptr<TextureHandle> pTextureHandle)
	: m_pVertexBufferHandle(pVertexBufferHandle)
	, m_pTextureHandle(pTextureHandle)
	, m_m4ModelMatrix(1.0)
	, m_drawMode(DrawMode::TRIANGLES)
	, m_lineWidth(1)
	, m_depthTestEnabled(true)
{
}

RenderData::RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, 
					shared_ptr<TextureHandle> pTextureHandle,
					DrawMode drawMode)
	: m_pVertexBufferHandle(pVertexBufferHandle)
	, m_pTextureHandle(pTextureHandle)
	, m_m4ModelMatrix(1.0)
	, m_drawMode(drawMode)
	, m_lineWidth(1)
	, m_depthTestEnabled(true)
{
}

RenderData *RenderData::setModelMatrix(glm::mat4 &m4ModelMatrix)
{
	m_m4ModelMatrix = m4ModelMatrix;
	return this;
}

RenderData *RenderData::setVertexBufferHandle(shared_ptr<VertexBufferHandle> handle)
{
	m_pVertexBufferHandle = handle;
	return this;
}

RenderData *RenderData::setTextureHandle(shared_ptr<TextureHandle> handle)
{
	m_pTextureHandle = handle;
	return this;
}

RenderData *RenderData::setDrawMode(DrawMode drawMode)
{
	m_drawMode = drawMode;
	return this;
}

shared_ptr<VertexBufferHandle> RenderData::getVertexBufferHandle()
{
	return m_pVertexBufferHandle;
}

shared_ptr<TextureHandle> RenderData::getTextureHandle()
{
	return m_pTextureHandle;
}

glm::mat4 &RenderData::getModelMatrix()
{
	return m_m4ModelMatrix;
}

DrawMode RenderData::getDrawMode()
{
	return m_drawMode;
}

void RenderData::setId(unsigned int id)
{
	m_id = id;
}

unsigned int RenderData::getId()
{
	return m_id;
}

void RenderData::setLineWidth(int width)
{
	m_lineWidth = width;
}

int RenderData::getLineWidth()
{
	return m_lineWidth;
}

void RenderData::setDepthTestEnabled(bool enabled)
{
	m_depthTestEnabled = enabled;
}

bool RenderData::isDepthTestEnabled()
{
	return m_depthTestEnabled;
}
