#pragma once

#include "Layer.hpp"

class DepthTest : public Layer
{
public:
	DepthTest();

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes);
};