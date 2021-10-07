#pragma once

#include "Layer.hpp"

class HasTangents : public Layer
{
public:
	HasTangents();

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};