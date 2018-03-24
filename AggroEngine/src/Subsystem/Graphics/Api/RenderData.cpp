#include "RenderData.hpp"


RenderData::RenderData()
	: m_pVertexBufferHandle()
	, m_m4ModelMatrix(1.0)
	, m_drawMode(DrawMode::TRIANGLES)
	, m_lineWidth(1)
	, m_depthTestEnabled(true)
	, m_lightingEnabled(true)
	, m_shadowsEnabled(true)
	, m_occlusionSize(0)
	, m_cullingEnabled(true)
{
}

RenderData::RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle)
	: m_pVertexBufferHandle(pVertexBufferHandle)
	, m_m4ModelMatrix(1.0)
	, m_drawMode(DrawMode::TRIANGLES)
	, m_lineWidth(1)
	, m_depthTestEnabled(true)
	, m_lightingEnabled(true)
	, m_shadowsEnabled(true)
	, m_occlusionSize(0)
	, m_cullingEnabled(true)
{
}

RenderData::RenderData(shared_ptr<VertexBufferHandle> pVertexBufferHandle, DrawMode drawMode)
	: m_pVertexBufferHandle(pVertexBufferHandle)
	, m_m4ModelMatrix(1.0)
	, m_drawMode(drawMode)
	, m_lineWidth(1)
	, m_depthTestEnabled(true)
	, m_lightingEnabled(true)
	, m_shadowsEnabled(true)
	, m_occlusionSize(0)
	, m_cullingEnabled(true)
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

RenderData *RenderData::setDrawMode(DrawMode drawMode)
{
	m_drawMode = drawMode;
	return this;
}

shared_ptr<VertexBufferHandle> RenderData::getVertexBufferHandle()
{
	return m_pVertexBufferHandle;
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

void RenderData::setMaterial(shared_ptr<Material> material)
{
	m_material = material;
}

shared_ptr<Material> RenderData::getMaterial()
{
	return m_material;
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

void RenderData::setLightingEnabled(bool enabled)
{
	m_lightingEnabled = enabled;
}

bool RenderData::isLightingEnabled()
{
	return m_lightingEnabled;
}

void RenderData::setShadowsEnabled(bool enabled)
{
	m_shadowsEnabled = enabled;
}

bool RenderData::isShadowsEnabled()
{
	return m_shadowsEnabled;
}

void RenderData::setOcclusionPoints(shared_ptr<glm::vec3> points, int size)
{
	m_occlusionPoints = points;
	m_occlusionSize = size;
}

shared_ptr<glm::vec3> RenderData::getOcclusionPoints()
{
	return m_occlusionPoints;
}

int RenderData::getOcclusionSize()
{
	return m_occlusionSize;
}

void RenderData::disableCulling()
{
	m_cullingEnabled = false;
}

bool RenderData::isCullingEnabled()
{
	return m_cullingEnabled;
}