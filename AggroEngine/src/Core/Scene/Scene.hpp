#pragma once

#include <memory>
#include <vector>
#include "../../Core/Object/Cameras/Camera.hpp"
#include "SceneNode.hpp"
#include "Listener.hpp"
using namespace std;

/**
 * Hierarchical structure of a scene (aka scene graph)
 *
 * author: wcrane
 * since: 2014-05-18
**/
class Scene
{
private:
	shared_ptr<SceneNode> m_root;
	shared_ptr<Camera> m_camera;
	Listener<Scene*> m_updateListeners;
	Listener<shared_ptr<SceneNode>> m_selectionChangeListeners;

	shared_ptr<SceneNode> m_selectedNode;

	void _applyToNodeRecursive(shared_ptr<SceneNode> node, std::function<void(SceneNode*)> func);

public:
	Scene();
	Scene(shared_ptr<SceneNode> root, shared_ptr<Camera> camera);

	shared_ptr<SceneNode> getRoot();
	shared_ptr<Camera> getCamera();

	void setRoot(shared_ptr<SceneNode> root);
	void setCamera(shared_ptr<Camera> camera);

	void addUpdateListener(std::function<void(Scene*)> listener);
	void update();

	void applyToAllNodes(std::function<void(SceneNode*)> func);

	void selectNode(shared_ptr<SceneNode> node);
	void deselectNode(shared_ptr<SceneNode> node);
	void deselectAllNodes();

	shared_ptr<SceneNode> getSelectedNode();

	void addSelectionChangeListener(std::function<void(shared_ptr<SceneNode>)> listener);

	void deleteNode(shared_ptr<SceneNode> node);
};