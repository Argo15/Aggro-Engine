#include "Command.hpp"

Command::Command()
	: m_renderNodes()
{
}

void Command::addRenderNode(shared_ptr<RenderNode> node)
{
	m_renderNodes.push_back(node);
}

vector<shared_ptr<RenderNode>> &Command::getRenderNodes()
{
	return m_renderNodes;
}

void Command::executeCommand()
{
}

void Command::end()
{
}