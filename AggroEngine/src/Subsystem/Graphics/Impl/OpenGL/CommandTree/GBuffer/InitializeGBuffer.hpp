#pragma once

#include "Layer.hpp"

class GBuffer;

class InitializeGBuffer : public Layer
{
private:
	GBuffer *m_gBuffer;

public:
	InitializeGBuffer(GBuffer *gBuffer);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};