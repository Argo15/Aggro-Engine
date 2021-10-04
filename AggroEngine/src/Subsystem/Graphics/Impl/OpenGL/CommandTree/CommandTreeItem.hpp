#pragma once

#include <unordered_map>
#include "Commands/Command.hpp"

class CommandTreeItem
{
private:
	vector<shared_ptr<Command>> m_commands;
	//unordered_map<Command, shared_ptr<Command>, CommandHash> m_existingCommands;

public:
	CommandTreeItem();

	void addCommand(shared_ptr<Command> command, shared_ptr<RenderNode> renderNode);
	vector<shared_ptr<Command>> &getCommands();
};