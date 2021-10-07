#include "DepthTest.hpp"
#include "EmptyCommand.hpp"
#include "DisableDepthCommand.hpp"

DepthTest::DepthTest()
{

}

shared_ptr<CommandTreeItem> DepthTest::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<Command> normalRender(new EmptyCommand());
	shared_ptr<Command> depthDisabledRender(new DisableDepthCommand());

	// No-depth render must be done after normal render, so it doesn't get drawn over later
	item->addCommand(normalRender, shared_ptr<RenderNode>());
	item->addCommand(depthDisabledRender, shared_ptr<RenderNode>()); 

	while (renderNodes)
	{
		shared_ptr<Command> command = renderNodes->getRenderData()->isDepthTestEnabled() ?
			normalRender : depthDisabledRender;
		item->addCommand(command, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}
