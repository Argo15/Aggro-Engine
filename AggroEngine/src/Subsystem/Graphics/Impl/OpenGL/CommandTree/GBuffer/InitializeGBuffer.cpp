#include "InitializeGBuffer.hpp"
#include "InitializeGBufferCommand.hpp"

InitializeGBuffer::InitializeGBuffer(GBuffer *gBuffer)
{
	m_gBuffer = gBuffer;
}

shared_ptr<CommandTreeItem> InitializeGBuffer::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<Command> initCommand(new InitializeGBufferCommand(
		m_gBuffer->getBuffer(), 
		m_gBuffer->getProgram()->getHandle(),
		m_gBuffer->getWidth(), 
		m_gBuffer->getHeight()));
	while (renderNodes)
	{
		item->addCommand(initCommand, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}
