#include "Scene.hpp"
#include "EngineContext.hpp"

Scene::Scene()
	: m_transformHook()
	, m_previewNode(new SceneNode(getNextId()))
{
}

Scene::Scene(shared_ptr<SceneNode> root, shared_ptr<Camera> camera)
	: m_root(root)
	, m_camera(camera)
	, m_transformHook()
	, m_previewNode(new SceneNode(getNextId()))
{
}

Scene::Scene(Chunk * const byteChunk, shared_ptr<EngineContext> context)
	: m_transformHook()
	, m_camera(new Camera())
	, m_previewNode(new SceneNode(getNextId()))
{
	m_camera = shared_ptr<Camera>(new Camera());
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	boost::unordered_map<int, shared_ptr<SceneNode>> baseMaterials;
	while (boost::optional<Chunk> nextChunk = parser.parseChunk())
	{
		if (*nextChunk->getType() == ChunkType::BASE_MATERIAL)
		{
			ByteParser matParser(*nextChunk);
			int matId = matParser.parseInt().get_value_or(-1);
			if (boost::optional<Chunk> matChunk = matParser.parseChunk())
			{
				shared_ptr<SceneNode> matNode = SceneNode::deserialize(&*matChunk, context, baseMaterials);
				addBaseMaterial(matNode);
				baseMaterials[matId] = matNode;
			}
		}
		else if (*nextChunk->getType() == ChunkType::SCENE_NODE)
		{
			m_root = SceneNode::deserialize(&*nextChunk, context, baseMaterials);
		}
		else if (*nextChunk->getType() == ChunkType::CAMERA)
		{
			m_camera = Camera::deserialize(nextChunk.get_ptr());
		}
	}
}

shared_ptr<Chunk> Scene::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes;

	// Base Materials
	shared_ptr<Chunk> matChunk;
	vector<shared_ptr<Chunk>> matChunks;
	for (int matId : m_baseMaterials | boost::adaptors::map_keys)
	{
		ByteAccumulator matBytes;
		matBytes.add(&matId);
		shared_ptr<Chunk> mateNodeChunk = m_baseMaterials[matId]->serialize(resources);
		matBytes.add(mateNodeChunk.get());
		matChunk = shared_ptr<Chunk>(new Chunk(ChunkType::BASE_MATERIAL, matBytes.getNumBytes(), matBytes.collect()));
		matChunks.push_back(matChunk);
		bytes.add(matChunk.get());
	}

	shared_ptr<Chunk> rootChunk = m_root->serialize(resources);
	shared_ptr<Chunk> cameraChunk = m_camera->serialize();
	bytes.add(rootChunk.get());
	bytes.add(cameraChunk.get());

	return shared_ptr<Chunk>(new Chunk(ChunkType::SCENE, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<Scene> Scene::deserialize(Chunk * const byteChunk, shared_ptr<EngineContext> context)
{
	if (*byteChunk->getType() != ChunkType::SCENE)
	{
		return shared_ptr<Scene>();
	}

	return shared_ptr<Scene>(new Scene(byteChunk, context));
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
	if (m_baseMaterials.find(id) != m_baseMaterials.end())
	{
		return m_baseMaterials[id];
	}
	return _getNodeByIdRecursive(m_root, id);
}

void Scene::addSelectionChangeListener(std::function<void(shared_ptr<SceneNode>)> listener)
{
	m_selectionChangeListeners.add(listener);
}

void Scene::deleteNode(shared_ptr<SceneNode> node)
{
	if (node->getParent())
	{
		node->getParent()->removeChild(node);
		node->notifyDeleted();
		return;
	}
	else if (m_baseMaterials.find(node->getId()) != m_baseMaterials.end())
	{
		m_baseMaterials.erase(node->getId());
		node->notifyDeleted();
	}
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

void Scene::addBaseMaterial(shared_ptr<SceneNode> node)
{
	m_baseMaterials[node->getId()] = node;
}

boost::unordered_map<int, shared_ptr<SceneNode>> &Scene::getBaseMaterials()
{
	return m_baseMaterials;
}

void Scene::setPreviewNode(shared_ptr<SceneNode> node)
{
	m_previewNode = node;
}

shared_ptr<SceneNode> Scene::getPreviewNode()
{
	return m_previewNode;
}

void Scene::update(shared_ptr<Selection> selection, shared_ptr<MouseState> mouse, shared_ptr<EngineContext> context)
{
	if (m_previewNode && m_previewNode->getTransformComponent())
	{
		const float hyperDepth = selection->getDepthVal();
		if (hyperDepth < 1)
		{
			const glm::mat4 projMat = context->getRenderOptions()->getProjectionMatrix();
			const glm::mat4 viewMat = context->getRenderOptions()->getViewMatrix();
			const glm::vec4 viewport = context->getRenderOptions()->getViewport();
			const int viewportWidth = (viewport[2] - viewport[0]);
			const int viewportHeight = (viewport[3] - viewport[1]);
			const float percentX = (float)mouse->getPosX() / viewportWidth;
			const float percentY = (float)mouse->getPosY() / viewportHeight;
			const glm::vec3 intersectPos = glm::unProject(glm::vec3(mouse->getPosX(), viewport[3] - mouse->getPosY(), hyperDepth), viewMat, projMat, viewport);
			m_previewNode->getTransformComponent()->setTranslate(intersectPos);
		}
		else
		{
			m_previewNode->getTransformComponent()->setTranslate(glm::vec3(0));
		}
	}
}