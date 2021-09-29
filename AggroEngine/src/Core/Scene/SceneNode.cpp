#include "SceneNode.hpp"
#include "StringUtil.hpp"
#include "Serialization.hpp"
#include "Scene.hpp"
#include <set>

SceneNode::SceneNode(unsigned int id, SceneNode *parent)
	: m_parent(parent)
	, m_children(shared_ptr<vector<shared_ptr<SceneNode>>>(new vector<shared_ptr<SceneNode>>()))
	, m_isSelected(false)
	, m_isBaseMaterialNode(false)
	, m_name("")
	, m_changeListeners()
	, m_deletedListeners()
	, m_id(id)
	, m_filename("")
{
	setName("Object");
}

SceneNode::SceneNode(Chunk * const byteChunk, shared_ptr<EngineContext> context, boost::unordered_map<int, shared_ptr<SceneNode>> baseMaterials, Scene *scene)
	: m_parent(nullptr)
	, m_children(shared_ptr<vector<shared_ptr<SceneNode>>>(new vector<shared_ptr<SceneNode>>()))
	, m_isSelected(false)
	, m_isBaseMaterialNode(false)
	, m_changeListeners()
	, m_deletedListeners()
{
	ByteParser parser = ByteParser(*byteChunk->getNumBytes(), byteChunk->getByteData().get());
	while (boost::optional<Chunk> nextChunk = parser.parseChunk())
	{
		if (*nextChunk->getType() == ChunkType::PRIMITIVES)
		{
			ByteParser primitives = ByteParser(*nextChunk);
			m_id = Scene::getNextId();
			setName(primitives.parseString().get_value_or("unknown"));
			m_isBaseMaterialNode = primitives.parseBool().get_value_or(false);
			m_filename = primitives.parseString().get_value_or("");
		}
		else if (*nextChunk->getType() == ChunkType::SCENE_NODE)
		{
			addChild(SceneNode::deserialize(nextChunk.get_ptr(), context, baseMaterials, scene));
		}
		else if (*nextChunk->getType() == ChunkType::TRANSFORM_COMPONENT)
		{
			m_transformComponent = TransformComponent::deserialize(nextChunk.get_ptr());
		}
		else if (*nextChunk->getType() == ChunkType::RENDER_COMPONENT)
		{
			m_renderComponent = RenderComponent::deserialize(nextChunk.get_ptr(), context->getResources());
			m_renderComponent->setSceneNode(this);
		}
		else if (*nextChunk->getType() == ChunkType::DIRECT_LIGHT_COMPONENT)
		{
			m_directLightComponent = DirectLightComponent::deserialize(nextChunk.get_ptr());
		}
		else if (*nextChunk->getType() == ChunkType::MATERIAL_COMPONENT)
		{
			m_materialComponent = MaterialComponent::deserialize(nextChunk.get_ptr(), this, context->getResources());
		}
		else if (*nextChunk->getType() == ChunkType::DELEGATE_MATERIAL)
		{
			ByteParser matBytes = ByteParser(*nextChunk);
			int baseId = matBytes.parseInt().get_value_or(-1);
			if (baseMaterials.find(baseId) != baseMaterials.end())
			{
				m_materialComponent = baseMaterials[baseId]->getMaterialComponent();
			}
		}
		else if (*nextChunk->getType() == ChunkType::MESH_COMPONENT)
		{
			m_meshComponent = MeshComponent::deserialize(nextChunk.get_ptr(), context->getResources(), context->getMeshCache(), context->getJobManager());
		}
		else if (*nextChunk->getType() == ChunkType::CAMERA_COMPONENT)
		{
			m_cameraComponent = CameraComponent::deserialize(nextChunk.get_ptr(), scene);
		}
	}
	if (m_filename != "")
	{
		int resourceId = context->getResources()->getIdForPath(m_filename);
		context->getSceneNodeCache()->getSceneNode(resourceId)->onReady([this](auto node) {
			resolveFileBackedData(node);
		});
	}
}

shared_ptr<Chunk> SceneNode::serialize(shared_ptr<Resources> resources)
{
	ByteAccumulator bytes;

	ByteAccumulator primitiveBytes;
	primitiveBytes.add(&m_name);
	primitiveBytes.add(&m_isBaseMaterialNode);
	primitiveBytes.add(&m_filename);
	bytes.add(shared_ptr<Chunk>(new Chunk(ChunkType::PRIMITIVES, primitiveBytes.getNumBytes(), primitiveBytes.collect())));

	for (auto & child : *m_children)
	{
		bytes.add(child->serialize(resources));
	}

	if (m_transformComponent)
	{
		bytes.add(m_transformComponent->serialize());
	}

	if (m_renderComponent)
	{
		bytes.add(m_renderComponent->serialize(resources));
	}

	if (m_directLightComponent)
	{
		bytes.add(m_directLightComponent->serialize());
	}

	if (m_materialComponent)
	{
		if (this == m_materialComponent->getOwner() || m_materialComponent->getOwner() == nullptr)
		{
			bytes.add(m_materialComponent->serialize(resources));
		}
		else
		{
			ByteAccumulator matBytes;
			int ownerId = m_materialComponent->getOwner()->getId();
			matBytes.add(&ownerId);
			bytes.add(new Chunk(ChunkType::DELEGATE_MATERIAL, matBytes.getNumBytes(), matBytes.collect()));
		}
	}

	if (m_meshComponent)
	{
		bytes.add(m_meshComponent->serialize(resources));
	}

	if (m_cameraComponent)
	{
		bytes.add(m_cameraComponent->serialize());
	}

	return shared_ptr<Chunk>(new Chunk(ChunkType::SCENE_NODE, bytes.getNumBytes(), bytes.collect()));
}

shared_ptr<SceneNode> SceneNode::deserialize(Chunk * const byteChunk, shared_ptr<EngineContext> context, boost::unordered_map<int, shared_ptr<SceneNode>> baseMaterials, Scene *scene)
{
	if (*byteChunk->getType() != ChunkType::SCENE_NODE)
	{
		return shared_ptr<SceneNode>();
	}

	return shared_ptr<SceneNode>(new SceneNode(byteChunk, context, baseMaterials, scene));
}

void SceneNode::resolveFileBackedData(shared_ptr<SceneNode> fileBackedSceneNode)
{
	if (!fileBackedSceneNode)
	{
		return;
	}
	if (m_meshComponent && fileBackedSceneNode->getMeshComponent())
	{
		m_meshComponent->setPrimaryMesh(fileBackedSceneNode->getMeshComponent()->getPrimaryMesh());
	}
	if (!m_children)
	{
		return;
	}
	boost::unordered_map<string, shared_ptr<SceneNode>> childNameToChild;
	for (shared_ptr<SceneNode> child : *m_children)
	{
		childNameToChild[child->getName()] = child;
	}
	for (shared_ptr<SceneNode> child : *fileBackedSceneNode->getChildren())
	{
		if (childNameToChild.find(child->getName()) != childNameToChild.end())
		{
			childNameToChild[child->getName()]->resolveFileBackedData(child);
		}
	}
}


shared_ptr<vector<shared_ptr<SceneNode>>> SceneNode::getChildren()
{
	return m_children;
}

void SceneNode::setSelected(bool selected)
{
	m_isSelected = selected;
}

bool SceneNode::isSelected()
{
	return m_isSelected;
}

void SceneNode::addChild(shared_ptr<SceneNode> child)
{
	m_children->push_back(child);
	child->setParent(this);
	m_changeListeners.notify(this);
}

void SceneNode::setName(string name)
{
	set<string> usedNames;
	if (m_parent != nullptr)
	{
		shared_ptr<vector<shared_ptr<SceneNode>>> siblings = m_parent->getChildren();
		if (siblings->size() > 0)
		{
			for (auto &sibling : *siblings.get())
			{
				if (sibling.get() != this)
				{
					usedNames.insert(sibling->getName());
				}
			}
		}
	}
	m_name = name;
	if (usedNames.size() > 0)
	{
		int count = 1;
		while (usedNames.find(m_name) != usedNames.end())
		{
			m_name = name + " (" + StringUtil::intToString(count++) + ")";
		}
	}
	m_changeListeners.notify(this);
}

string SceneNode::getName()
{
	return m_name;
}

void SceneNode::setFilename(string filename)
{
	m_filename = filename;
}

unsigned int SceneNode::getId()
{
	return m_id;
}

string SceneNode::getFilename()
{
	return m_filename;
}

void SceneNode::setParent(SceneNode *parent)
{
	m_parent = parent;
	setName(m_name); // Changes name if it's shared with siblings
}

SceneNode *SceneNode::getParent()
{
	return m_parent;
}

void SceneNode::addChangeListener(void *ns, std::function<void(SceneNode *)> listener)
{
	m_changeListeners.add(ns, listener);
}

void SceneNode::removeChangeListener(void *ns)
{
	m_changeListeners.remove(ns);
}

void SceneNode::notifyChanged()
{
	m_changeListeners.notify(this);
}

void SceneNode::addDeletedListener(void *ns, std::function<void(SceneNode *)> listener)
{
	m_deletedListeners.add(ns, listener);
}

void SceneNode::notifyDeleted()
{
	m_deletedListeners.notify(this);
}

void SceneNode::removeChild(shared_ptr<SceneNode> child)
{
	m_children->erase(remove(m_children->begin(), m_children->end(), child), m_children->end());
}

glm::mat4 SceneNode::getWorldTransform(glm::vec3 objCenter)
{
	glm::mat4 tansform = glm::translate(getObjectTransform(), objCenter);
	if (m_parent)
	{
		return m_parent->getWorldTransform() * tansform;
	}
	return tansform;
}

glm::mat4 SceneNode::getOrthogonalWorldTransform()
{
	glm::mat4 tansform = getOrthogonalObjectTransform();
	if (m_parent)
	{
		return m_parent->getOrthogonalWorldTransform() * tansform;
	}
	return tansform;
}

glm::vec3 SceneNode::getWorldTranslate()
{
	glm::mat4 tansform = getWorldTransform();
	return glm::vec3(tansform[3][0], tansform[3][1], tansform[3][2]);
}

glm::mat4 SceneNode::getParentTransform()
{
	glm::mat4 tansform = glm::mat4(1.0);
	if (m_parent)
	{
		return m_parent->getWorldTransform() * tansform;
	}
	return tansform;
}

glm::mat4 SceneNode::getOrthogonalParentTransform()
{
	glm::mat4 tansform = glm::mat4(1.0);
	if (m_parent)
	{
		return m_parent->getOrthogonalWorldTransform() * tansform;
	}
	return tansform;
}

glm::mat4 SceneNode::getObjectTransform()
{
	if (!m_transformComponent)
	{
		return glm::mat4(1.0);
	}
	return m_transformComponent->getTransform();
}

glm::mat4 SceneNode::getOrthogonalObjectTransform()
{
	if (!m_transformComponent)
	{
		return glm::mat4(1.0);
	}
	glm::mat4 rotate(m_transformComponent->getRotate());
	glm::vec4 translate = glm::vec4(m_transformComponent->getTranslate(), 1.0f);
	rotate[3] = translate;
	return rotate;
}

glm::vec3 SceneNode::getMeshCenter()
{
	if (!m_meshComponent)
	{
		return glm::vec3(0);
	}
	return m_meshComponent->getMeshCenter();
}

bool SceneNode::hasTransformComponent()
{
	return m_transformComponent != nullptr;
}

void SceneNode::setTransformComponent(shared_ptr<TransformComponent> transformComponent)
{
	m_transformComponent = transformComponent;
	notifyChanged();
}

shared_ptr<TransformComponent> SceneNode::getTransformComponent()
{
	return m_transformComponent;
}

bool SceneNode::hasRenderComponent()
{
	return m_renderComponent && m_renderComponent != nullptr;
}

void SceneNode::setRenderComponent(shared_ptr<RenderComponent> renderComponent)
{
	renderComponent->setSceneNode(this);
	m_renderComponent = renderComponent;
	notifyChanged();
}

shared_ptr<RenderComponent> SceneNode::getRenderComponent()
{
	return m_renderComponent;
}

bool SceneNode::hasDirectLightComponent()
{
	return m_directLightComponent != nullptr;
}

void SceneNode::setDirectLightComponent(shared_ptr<DirectLightComponent> directLightComponent)
{
	m_directLightComponent = directLightComponent;
	notifyChanged();
}

shared_ptr<DirectLightComponent> SceneNode::getDirectLightComponent()
{
	return m_directLightComponent;
}

bool SceneNode::hasMaterialComponent()
{
	return m_materialComponent != nullptr;
}

void SceneNode::setMaterialComponent(shared_ptr<MaterialComponent> materialComponent)
{
	m_materialComponent = materialComponent;
	notifyChanged();
}

shared_ptr<MaterialComponent> SceneNode::getMaterialComponent()
{
	return m_materialComponent;
}

void SceneNode::setBaseMaterialNode(bool value)
{
	m_isBaseMaterialNode = value;
}

bool SceneNode::isBaseMaterialNode()
{
	return hasMaterialComponent() && m_isBaseMaterialNode;
}

bool SceneNode::hasMeshComponent()
{
	return m_meshComponent != nullptr;
}

void SceneNode::setMeshComponent(shared_ptr<MeshComponent> meshComponent)
{
	m_meshComponent = meshComponent;
	notifyChanged();
}

shared_ptr<MeshComponent> SceneNode::getMeshComponent()
{
	return m_meshComponent;
}

bool SceneNode::hasCameraComponent()
{
	return m_cameraComponent != nullptr;
}

void SceneNode::setCameraComponent(shared_ptr<CameraComponent> cameraComponent)
{
	m_cameraComponent = cameraComponent;
	notifyChanged();
}

shared_ptr<CameraComponent> SceneNode::getCameraComponent()
{
	return m_cameraComponent;
}

shared_ptr<Camera> SceneNode::getCamera()
{
	if (m_cameraComponent && m_transformComponent)
	{
		return m_cameraComponent->getCamera(m_transformComponent);
	}
	return shared_ptr<Camera>();
}