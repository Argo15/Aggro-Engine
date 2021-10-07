#include "FilterShadowLayer.hpp"
#include "EmptyCommand.hpp"

FilterShadowLayer::FilterShadowLayer(shared_ptr<BufferSyncContext> syncContext)
	: m_syncContext(syncContext)
{

}

shared_ptr<CommandTreeItem> FilterShadowLayer::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<Command> emptyCommand(new EmptyCommand());

	const shared_ptr<Frustrum> frustrum = renderOptions.getFrustrum();

	while (renderNodes)
	{
		shared_ptr<RenderData> renderData = renderNodes->getRenderData();
		shared_ptr<VertexBufferHandle> vboHandle = renderData->getVertexBufferHandle();

		if (!vboHandle ||
			!renderData->isDepthTestEnabled() ||
			!renderData->isShadowsEnabled() ||
			!m_syncContext->checkAndClearSync(vboHandle->getVertexHandle()))
		{
			renderNodes = renderNodes->next();
			continue;
		}

		item->addCommand(emptyCommand, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}