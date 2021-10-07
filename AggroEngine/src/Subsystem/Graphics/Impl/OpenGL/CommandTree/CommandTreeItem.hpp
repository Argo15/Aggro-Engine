#pragma once

#include <unordered_map>
#include "Commands/Command.hpp"

class CommandTreeItem
{
private:
	vector<shared_ptr<Command>> m_commands;

public:
	CommandTreeItem();

	void addCommand(shared_ptr<Command> command, shared_ptr<RenderNode> renderNode, bool hasDuplicates = true);
	vector<shared_ptr<Command>> &getCommands();
};