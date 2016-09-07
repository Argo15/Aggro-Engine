#include "Scene.hpp"
#include "FunctionUtil.hpp"

Scene::Scene()
{
}

Scene::Scene(shared_ptr<SceneNode> root, shared_ptr<Camera> camera)
	: m_root(root)
	, m_camera(camera)
{
}

shared_ptr<SceneNode> Scene::getRoot()
{
	return m_root;
}

shared_ptr<Camera> Scene::getCamera()
{
	return m_camera;
}

void Scene::setRoot(shared_ptr<SceneNode> root)
{
	m_root = root;
}

void Scene::setCamera(shared_ptr<Camera> camera)
{
	m_camera = camera;
}

void Scene::addUpdateListener(std::function<void(Scene*)> listener)
{
	m_updateListeners.add(listener);
}

void Scene::update()
{
	m_updateListeners.notify(this);
}

void Scene::applyToAllNodes(std::function<void(SceneNode*)> func)
{
	_applyToNodeRecursive(m_root, func);
}

void Scene::_applyToNodeRecursive(shared_ptr<SceneNode> node, std::function<void(SceneNode*)> func)
{
	func(node.get());
	shared_ptr<vector<shared_ptr<SceneNode>>> children = node->getChildren();
	if (children->size() > 0)
	{
		for (auto &child : *children.get())
		{
			_applyToNodeRecursive(child, func);
		}
	}
}

void Scene::selectNode(shared_ptr<SceneNode> node)
{
	node->setSelected(true);
	m_selectedNode = node;
	m_selectionChangeListeners.notify(m_selectedNode);
}

void Scene::deselectNode(shared_ptr<SceneNode> node)
{
	node->setSelected(false);
	if (m_selectedNode == node)
	{
		m_selectedNode = shared_ptr<SceneNode>();
		m_selectionChangeListeners.notify(m_selectedNode);
	}
}

void Scene::deselectAllNodes()
{
	applyToAllNodes([](auto node) {node->setSelected(false);});
	m_selectedNode = shared_ptr<SceneNode>();
	m_selectionChangeListeners.notify(m_selectedNode);
}

shared_ptr<SceneNode> Scene::getSelectedNode()
{
	return m_selectedNode;
}

void Scene::addSelectionChangeListener(std::function<void(shared_ptr<SceneNode>)> listener)
{
	m_selectionChangeListeners.add(listener);
}