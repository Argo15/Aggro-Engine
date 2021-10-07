#include "Command.hpp"

Command::Command()
	: m_renderNodes()
	, m_hash(rand())
{
}

void Command::addRenderNode(shared_ptr<RenderNode> node)
{
	shared_ptr<RenderNode> newNode(new RenderNode(node->getRenderData(), node->getId()));
	if (m_renderNodes)
	{
		newNode->setNext(m_renderNodes);
	}
	m_renderNodes = newNode;
}

shared_ptr<RenderNode> Command::getRenderNodes()
{
	return m_renderNodes;
}

void Command::executeCommand()
{
}

void Command::end()
{
}