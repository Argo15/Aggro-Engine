#include "FilterLayer.hpp"
#include "EmptyCommand.hpp"

FilterLayer::FilterLayer(shared_ptr<BufferSyncContext> syncContext)
	: m_syncContext(syncContext)
{

}

shared_ptr<CommandTreeItem> FilterLayer::getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<Command> emptyCommand(new EmptyCommand());

	const shared_ptr<Frustrum> frustrum = renderOptions.getFrustrum();

	for (int i = 0; i<renderNodes.size(); i++)
	{
		shared_ptr<RenderData> renderData = renderNodes[i]->getRenderData();
		shared_ptr<VertexBufferHandle> vboHandle = renderData->getVertexBufferHandle();
		if (!vboHandle)
		{
			continue;
		}

		if (!m_syncContext->checkAndClearSync(vboHandle->getVertexHandle()))
		{
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
				continue;
			}
		}

		item->addCommand(emptyCommand, renderNodes[i]);
	}
	return item;
}