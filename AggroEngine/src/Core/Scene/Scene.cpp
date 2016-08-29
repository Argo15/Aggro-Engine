#include "Scene.hpp"

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
	m_updateListeners.push_back(listener);
}

void Scene::update()
{
	if (m_updateListeners.size() > 0)
	{
		for (auto &listener : m_updateListeners)
		{
			listener(this);
		}
	}
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

void Scene::deselectAllNodes()
{
	applyToAllNodes([](auto node) {node->setSelected(false);});
}