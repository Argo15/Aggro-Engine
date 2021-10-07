#include "HasTangents.hpp"
#include "HasTangentsCommand.hpp"

HasTangents::HasTangents()
{

}

shared_ptr<CommandTreeItem> HasTangents::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<Command> hasTangets(new HasTangentsCommand(true));
	shared_ptr<Command> noTangets(new HasTangentsCommand(false));

	const shared_ptr<Frustrum> frustrum = renderOptions.getFrustrum();

	while (renderNodes)
	{
		shared_ptr<RenderData> renderData = renderNodes->getRenderData();
		shared_ptr<VertexBufferHandle> vboHandle = renderData->getVertexBufferHandle();
		item->addCommand(vboHandle->hasTangents() ? hasTangets : noTangets, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}