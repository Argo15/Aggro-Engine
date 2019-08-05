#pragma once

#include "SceneNodeImporter.hpp"
#include "Listener.hpp"
#include <memory>
using namespace std;

class CachedSceneNodeHandler
{
private:
	shared_ptr<SceneNode> m_sceneNode;
	Listener<shared_ptr<SceneNode>> m_readyListeners;

public:
	CachedSceneNodeHandler(shared_ptr<SceneNode> sceneNode = nullptr);

	shared_ptr<SceneNode> get();
	void updateSceneNode(shared_ptr<SceneNode> sceneNode);

	void notifyReady();
	void onReady(function<void(shared_ptr<SceneNode>)> func);
};