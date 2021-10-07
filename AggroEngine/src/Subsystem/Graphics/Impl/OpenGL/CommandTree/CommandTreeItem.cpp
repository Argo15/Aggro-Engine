#include "CommandTreeItem.hpp"

CommandTreeItem::CommandTreeItem()
{
	m_commands.reserve(100);
}

void CommandTreeItem::addCommand(shared_ptr<Command> command, shared_ptr<RenderNode> renderNode, bool hasDuplicates)
{
	shared_ptr<Command> existing;
	if (hasDuplicates)
	{
		for (int i = 0; i < m_commands.size(); i++)
		{
			if (command->equals(m_commands[i]))
			{
				existing = m_commands[i];
				break;
			}
		}
	}
	if (!existing)
	{
		m_commands.push_back(command);
		existing = command;
	}
	if (renderNode)
	{
		existing->addRenderNode(renderNode);
	}
}

vector<shared_ptr<Command>> &CommandTreeItem::getCommands()
{
	return m_commands;
}