#include "RenderOptions.hpp"

static glm::mat4 defaultProjMat = glm::ortho(0.f,1.f,1.f,0.f,-1.f,1.f);
static glm::mat4 defaultViewMat = glm::lookAt(glm::vec3(0.f,0.f,2.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
static glm::vec4 defaultViewport = glm::vec4(0, 0, 1000, 1000);

RenderOptions::RenderOptions()
	: m_renderTarget(SHADED)
	, m_m4ProjectionMatrix(defaultProjMat)
	, m_m4ViewMatrix(defaultViewMat)
	, m_v4Viewport(defaultViewport)
{
}

void RenderOptions::clear()
{
	m_m4ProjectionMatrix = defaultProjMat;
	m_m4ViewMatrix = defaultViewMat;
	m_v4Viewport = defaultViewport;
	m_directLight = shared_ptr<DirectLight>();
}

RenderOptions &RenderOptions::setProjectionMatrix(glm::mat4 &m4Projection)
{
	m_m4ProjectionMatrix = m4Projection;
	return *this;
}
	
RenderOptions &RenderOptions::setViewMatrix(glm::mat4 &m4View)
{
	m_m4ViewMatrix = m4View;
	return *this;
}

RenderOptions &RenderOptions::setViewport(glm::vec4 &m4Viewport)
{
	m_v4Viewport = m4Viewport;
	return *this;
}

const glm::mat4 &RenderOptions::getProjectionMatrix()
{
	return m_m4ProjectionMatrix;
}

const glm::mat4 &RenderOptions::getViewMatrix()
{
	return m_m4ViewMatrix;
}

const glm::vec4 &RenderOptions::getViewport()
{
	return m_v4Viewport;
}

void RenderOptions::setRenderTarget(RenderTarget target)
{
	m_renderTarget = target;
}

RenderOptions::RenderTarget RenderOptions::getRenderTarget()
{
	return m_renderTarget;
}

bool RenderOptions::RenderOptions::hasDirectLight()
{
	return m_directLight != nullptr;
}

void RenderOptions::addDirectLight(shared_ptr<DirectLight> directLight)
{
	m_directLight = directLight;
}

shared_ptr<DirectLight> RenderOptions::getDirectLight()
{
	return m_directLight;
}