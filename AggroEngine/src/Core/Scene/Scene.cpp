#include "Scene.hpp"

Scene::Scene()
	: m_transformHook()
{
}

Scene::Scene(shared_ptr<SceneNode> root, shared_ptr<Camera> camera)
	: m_root(root)
	, m_camera(camera)
	, m_transformHook()
{
}

Scene::Scene(Chunk * const byteChunk, shared_ptr<Resources> resources)
	: m_camera(shared_ptr<Camera>(new Camera()))
	, m_transformHook()
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	if (boost::optional<Chunk> nextChunk = parser.parseChunk())
	{
		if (*nextChunk->getType() == ChunkType::SCENE_NODE)
		{
			m_root = SceneNode::deserialize(&*nextChunk, resources);
		}
	}
}

shared_ptr<Chunk> Scene::serialize(shared_ptr<Resources> resources)
{
	shared_ptr<Chunk> rootChunk = m_root->serialize(resources);
	ByteAccumulator bytes;
	bytes.add(rootChunk.get());
	return shared_ptr<Chunk>(new Chunk(ChunkType::SCENE, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<Scene> Scene::deserialize(Chunk * const byteChunk, shared_ptr<Resources> resources)
{
	if (*byteChunk->getType() != ChunkType::SCENE)
	{
		return shared_ptr<Scene>();
	}

	return shared_ptr<Scene>(new Scene(byteChunk, resources));
}


shared_ptr<SceneNode> Scene::getRoot()
{
	return m_root;
}

shared_ptr<Camera> Scene::getCamera()
{
	return m_camera;
}

void Scene::setTransformHook(shared_ptr<TransformHook> transformHook)
{
	m_transformHook = transformHook;
}

shared_ptr<TransformHook> Scene::getTransformHook()
{
	return m_transformHook;
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

void Scene::selectNodeById(unsigned int id)
{
	deselectAllNodes();
	shared_ptr<SceneNode> node = getNodeById(id);
	if (node && node->getId() == id)
	{
		selectNode(node);
	}
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

shared_ptr<SceneNode> Scene::_getNodeByIdRecursive(shared_ptr<SceneNode> node, unsigned int id)
{
	if (!node)
	{
		return shared_ptr<SceneNode>();
	}
	if (node->getId() == id)
	{
		return node;
	}
	shared_ptr<vector<shared_ptr<SceneNode>>> children = node->getChildren();
	if (children->size() > 0)
	{
		for (auto &child : *children.get())
		{
			shared_ptr<SceneNode> maybeNode = _getNodeByIdRecursive(child, id);
			if (maybeNode && maybeNode->getId() == id)
			{
				return maybeNode;
			}
		}
	}
	return shared_ptr<SceneNode>();
}

shared_ptr<SceneNode> Scene::getNodeById(unsigned int id)
{
	return _getNodeByIdRecursive(m_root, id);
}

void Scene::addSelectionChangeListener(std::function<void(shared_ptr<SceneNode>)> listener)
{
	m_selectionChangeListeners.add(listener);
}

void Scene::deleteNode(shared_ptr<SceneNode> node)
{
	node->getParent()->removeChild(node);
	node->notifyDeleted();
}

void Scene::deleteSelectedNode()
{
	if (m_selectedNode)
	{
		deleteNode(m_selectedNode);
	}
	m_selectedNode = shared_ptr<SceneNode>();
}

int Scene::s_nextId = 1;
unsigned int Scene::getNextId()
{
	return s_nextId++;
}

int Scene::_getMaxNodeIdRecursive(shared_ptr<SceneNode> node)
{
	if (!node)
	{
		return 0;
	}
	int maxId = node->getId();
	shared_ptr<vector<shared_ptr<SceneNode>>> children = node->getChildren();
	if (children->size() > 0)
	{
		for (auto &child : *children.get())
		{
			maxId = max(maxId, _getMaxNodeIdRecursive(child));
		}
	}
	return maxId;
}