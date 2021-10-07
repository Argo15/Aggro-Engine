#include "DrawShadowElements.hpp"
#include "DrawShadowElementsCommand.hpp"

DrawShadowElements::DrawShadowElements(ShadowMapBuffer *shadowMapBuf)
	: m_shadowMapBuf(shadowMapBuf)
{

}

shared_ptr<CommandTreeItem> DrawShadowElements::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());

	while (renderNodes)
	{
		shared_ptr<Command> drawCommand(new DrawShadowElementsCommand(
			renderNodes->getRenderData(), 
			m_shadowMapBuf->getProgram()));

		item->addCommand(drawCommand, renderNodes, false);
		renderNodes = renderNodes->next();
	}
	return item;
}