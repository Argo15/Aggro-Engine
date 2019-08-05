#include "CachedSceneNodeHandler.hpp"

CachedSceneNodeHandler::CachedSceneNodeHandler(shared_ptr<SceneNode> sceneNode)
	: m_sceneNode(sceneNode)
{

}

shared_ptr<SceneNode> CachedSceneNodeHandler::get()
{
	return m_sceneNode;
}

void CachedSceneNodeHandler::updateSceneNode(shared_ptr<SceneNode> sceneNode)
{
	m_sceneNode = sceneNode;
}

void CachedSceneNodeHandler::notifyReady()
{
	m_readyListeners.notify(m_sceneNode);
	m_readyListeners.remove();
}

void CachedSceneNodeHandler::onReady(function<void(shared_ptr<SceneNode>)> func)
{
	if (m_sceneNode)
	{
		func(m_sceneNode);
	}
	else
	{
		m_readyListeners.add(func);
	}
}