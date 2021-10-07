#include "CommandTree.hpp"

CommandTree::CommandTree()
{

}

void CommandTree::addLayer(shared_ptr<Layer> layer)
{
	m_layers.push_back(layer);
}

void CommandTree::execute(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain)
{
	_executeLayer(renderOptions, renderChain->getFirst(), 0);
}

void CommandTree::_executeLayer(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes, int layerIdx)
{
	if (m_layers.size() <= layerIdx || !renderNodes)
	{
		return;
	}
	shared_ptr<Layer> layer = m_layers[layerIdx];

	;
	shared_ptr<CommandTreeItem> commandItem = layer->getCommands(renderOptions, renderNodes);
	vector<shared_ptr<Command>> commands = commandItem->getCommands();
	for (int i = 0; i < commands.size(); i++)
	{
		commands[i]->executeCommand();
		_executeLayer(renderOptions, commands[i]->getRenderNodes(), layerIdx + 1);
		commands[i]->end();
	}
}
