#include "OpenGLRenderHandle.hpp"

OpenGLRenderHandle::OpenGLRenderHandle(int nodeId, shared_ptr<RenderData> renderData, shared_ptr<RenderChain> renderChain)
	: RenderHandle()
	, m_nodeId(nodeId)
	, m_renderData(renderData)
	, m_renderChain(renderChain)
	, m_unstaged(false)
{
}

OpenGLRenderHandle::~OpenGLRenderHandle() 
{
	unstageRender();
}

shared_ptr<RenderData> OpenGLRenderHandle::getRenderData()
{
	return m_renderData;
}

void OpenGLRenderHandle::unstageRender()
{
	if (!m_unstaged) {
		m_renderChain->removeRenderData(m_nodeId);
		m_unstaged = true;
	}
}

void OpenGLRenderHandle::restageRender()
{
	if (m_unstaged) {
		m_nodeId = m_renderChain->appendRenderData(m_renderData);
		m_unstaged = false;
	}
}