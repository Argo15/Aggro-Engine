#include "FilterLayer.hpp"
#include "EmptyCommand.hpp"

FilterLayer::FilterLayer(shared_ptr<BufferSyncContext> syncContext)
	: m_syncContext(syncContext)
{

}

shared_ptr<CommandTreeItem> FilterLayer::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<Command> emptyCommand(new EmptyCommand());

	const shared_ptr<Frustrum> frustrum = renderOptions.getFrustrum();

	while (renderNodes)
	{
		shared_ptr<RenderData> renderData = renderNodes->getRenderData();
		shared_ptr<VertexBufferHandle> vboHandle = renderData->getVertexBufferHandle();
		if (!vboHandle)
		{
			renderNodes = renderNodes->next();
			continue;
		}

		if (!m_syncContext->checkAndClearSync(vboHandle->getVertexHandle()))
		{
			renderNodes = renderNodes->next();
			continue;
		}

		// slow, 100 fps -> 85 fps. (about 2ms)
		// That's when we comment out the next layers, so we aren't seeing the benefits 
		// Culling on top of rendering sponza brings us from 60 fps -> 80 fps
		if (renderData->isCullingEnabled() && renderData->getOcclusionPoints())
		{
			FrustrumCulling culling = frustrum->getCulling(
				renderData->getOcclusionPoints(),
				renderData->getOcclusionSize(),
				renderData->getModelMatrix());
			if (culling == OUTSIDE)
			{
				renderNodes = renderNodes->next();
				continue;
			}
		}

		item->addCommand(emptyCommand, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}