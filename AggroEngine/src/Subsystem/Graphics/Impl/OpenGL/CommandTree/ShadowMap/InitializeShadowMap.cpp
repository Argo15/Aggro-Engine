#include "InitializeShadowMap.hpp"
#include "InitializeShadowMapCommand.hpp"
#include "Config.hpp"

InitializeShadowMap::InitializeShadowMap(ShadowMapBuffer *shadowMapBuf)
	: m_shadowMapBuf(shadowMapBuf)
{
}

shared_ptr<CommandTreeItem> InitializeShadowMap::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	shared_ptr<DirectLight> light = renderOptions.getDirectLight();
	if (!light)
	{
		return item;
	}

	shared_ptr<Command> shadowMapInit(new InitializeShadowMapCommand(m_shadowMapBuf));
	while (renderNodes)
	{
		item->addCommand(shadowMapInit, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}
