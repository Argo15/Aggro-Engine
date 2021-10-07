#pragma once

#include "Layer.hpp"
#include "ShadowMapBuffer.hpp"

class MVPMatrix : public Layer
{
private:
	ShadowMapBuffer *m_shadowMapBuf;

public:
	MVPMatrix(ShadowMapBuffer *shadowMapBuf);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};