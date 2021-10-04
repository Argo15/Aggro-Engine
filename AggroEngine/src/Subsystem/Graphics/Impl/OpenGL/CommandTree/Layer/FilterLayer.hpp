#pragma once

#include "Layer.hpp"
#include "BufferSyncContext.hpp"

class FilterLayer : public Layer
{
private:
	shared_ptr<BufferSyncContext> m_syncContext;

public:
	FilterLayer(shared_ptr<BufferSyncContext> syncContext);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes);
};