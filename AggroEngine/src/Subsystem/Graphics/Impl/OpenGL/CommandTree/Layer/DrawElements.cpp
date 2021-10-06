#include "DrawElements.hpp"
#include "DrawElementsCommand.hpp"

DrawElements::DrawElements(shared_ptr<GLSLProgram> glslProgram)
	: m_glslProgram(glslProgram)
{

}

shared_ptr<CommandTreeItem> DrawElements::getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());

	for (int i = 0; i<renderNodes.size(); i++)
	{
		shared_ptr<Command> drawCommand(new DrawElementsCommand(renderNodes[i]->getRenderData(), m_glslProgram));
		item->addCommand(drawCommand, renderNodes[i]);
	}
	return item;
}