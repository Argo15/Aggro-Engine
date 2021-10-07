#pragma once

#include <memory>
#include "CommandTree/CommandTreeItem.hpp"
#include "RenderOptions.hpp"
#include "RenderNode.hpp"

class Layer
{
public:
	virtual shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes) = 0;
};