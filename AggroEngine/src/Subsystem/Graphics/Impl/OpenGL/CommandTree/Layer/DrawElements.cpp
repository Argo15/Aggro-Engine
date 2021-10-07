#include "DrawElements.hpp"
#include "DrawElementsCommand.hpp"

DrawElements::DrawElements(shared_ptr<GLSLProgram> glslProgram)
	: m_glslProgram(glslProgram)
{

}

shared_ptr<CommandTreeItem> DrawElements::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());

	while (renderNodes)
	{
		shared_ptr<Command> drawCommand(new DrawElementsCommand(renderNodes->getRenderData(), m_glslProgram));
		item->addCommand(drawCommand, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}