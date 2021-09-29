#pragma once

#include <memory>
#include "RenderHandle.hpp"
#include "RenderData.hpp"
#include "RenderChain.hpp"

class OpenGLRenderHandle : public RenderHandle
{
private:
	int m_nodeId;
	shared_ptr<RenderData> m_renderData;
	shared_ptr<RenderChain> m_renderChain;
	bool m_unstaged;

public:
	OpenGLRenderHandle(int nodeId, shared_ptr<RenderData> renderData, shared_ptr<RenderChain> renderChain);
	~OpenGLRenderHandle();

	shared_ptr<RenderData> getRenderData();

	void unstageRender();
	void restageRender();
};