#pragma once

#include "Layer.hpp"
#include "ShadowMapBuffer.hpp"

class DrawShadowElements : public Layer
{
private:
	ShadowMapBuffer *m_shadowMapBuf;

public:
	DrawShadowElements(ShadowMapBuffer *shadowMapBuf);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};