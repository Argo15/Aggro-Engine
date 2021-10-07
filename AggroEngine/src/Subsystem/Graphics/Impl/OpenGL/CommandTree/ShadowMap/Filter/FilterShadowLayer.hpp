#pragma once

#include "Layer.hpp"
#include "BufferSyncContext.hpp"

class FilterShadowLayer : public Layer
{
private:
	shared_ptr<BufferSyncContext> m_syncContext;

public:
	FilterShadowLayer(shared_ptr<BufferSyncContext> syncContext);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};