#include "InitializeGBuffer.hpp"
#include "Commands/InitializeGBufferCommand.hpp"

InitializeGBuffer::InitializeGBuffer(GBuffer *gBuffer)
{
	m_gBuffer = gBuffer;
}

shared_ptr<CommandTreeItem> InitializeGBuffer::getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<Command> initCommand(new InitializeGBufferCommand(
		m_gBuffer->getBuffer(), 
		m_gBuffer->getProgram()->getHandle(),
		m_gBuffer->getWidth(), 
		m_gBuffer->getHeight()));
	for (int i=0; i<renderNodes.size(); i++)
	{
		item->addCommand(initCommand, renderNodes[i]);
	}
	return item;
}
