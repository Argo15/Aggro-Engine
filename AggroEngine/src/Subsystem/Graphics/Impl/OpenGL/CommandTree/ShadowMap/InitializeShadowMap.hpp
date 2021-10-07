#pragma once

#include "Layer.hpp"

class ShadowMapBuffer;

class InitializeShadowMap : public Layer
{
private:
	ShadowMapBuffer *m_shadowMapBuf;

public:
	InitializeShadowMap(ShadowMapBuffer *m_shadowMapBuf);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};