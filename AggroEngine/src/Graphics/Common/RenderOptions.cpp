#include "RenderOptions.hpp"
#include "Config.hpp"

static glm::mat4 defaultProjMat = glm::ortho(0.f,1.f,1.f,0.f,-1.f,1.f);
static glm::mat4 defaultViewMat = glm::lookAt(glm::vec3(0.f,0.f,2.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));

RenderOptions::RenderOptions()
	: m_renderMode(SHADED)
	, m_m4ProjectionMatrix(defaultProjMat)
	, m_m4ViewMatrix(defaultViewMat)
{
	const Properties& props = gConfig->getProperties();
	vector<int> nDimensions = props.getIntArrayProperty("graphics.resolution");
	resWidth = nDimensions[0];
	resHeight = nDimensions[1];
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

glm::mat4 &RenderOptions::getProjectionMatrix()
{
	return m_m4ProjectionMatrix;
}

glm::mat4 &RenderOptions::getViewMatrix()
{
	return m_m4ViewMatrix;
}

void RenderOptions::setRenderMode(RenderMode mode)
{
	m_renderMode = mode;
}

RenderOptions::RenderMode RenderOptions::getRenderMode()
{
	return m_renderMode;
}

int RenderOptions::getResolutionWidth()
{
	return resWidth;
}

int RenderOptions::getResolutionHeight()
{
	return resHeight;
}