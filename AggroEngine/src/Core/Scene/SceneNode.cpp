#include "SceneNode.hpp"
#include "StringUtil.hpp"
#include <set>

SceneNode::SceneNode(unsigned int id, SceneNode *parent)
	: m_parent(parent)
	, m_children(shared_ptr<vector<shared_ptr<SceneNode>>>(new vector<shared_ptr<SceneNode>>()))
	, m_isSelected(false)
	, m_name("")
	, m_changeListeners()
	, m_deletedListeners()
	, m_id(id)
{
	setName("Object");
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
				usedNames.insert(sibling->getName());
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

unsigned int SceneNode::getId()
{
	return m_id;
}

void SceneNode::setParent(SceneNode *parent)
{
	m_parent = parent;
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

glm::mat4 SceneNode::getWorldTransform()
{
	glm::mat4 tansform = getObjectTransform();
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
	glm::vec4 translate = glm::vec4(*m_transformComponent->getTranslate(), 1.0f);
	rotate[3] = translate;
	return rotate;
}

bool SceneNode::hasTransformComponent()
{
	return m_transformComponent != nullptr;
}

void SceneNode::setTransformComponent(shared_ptr<TransformComponent> transformComponent)
{
	m_transformComponent = transformComponent;
}

shared_ptr<TransformComponent> SceneNode::getTransformComponent()
{
	return m_transformComponent;
}

bool SceneNode::hasRenderComponent()
{
	return m_renderComponent != nullptr;
}

void SceneNode::setRenderComponent(shared_ptr<RenderComponent> renderComponent)
{
	m_renderComponent = renderComponent;
}

shared_ptr<RenderComponent> SceneNode::getRenderComponent()
{
	return m_renderComponent;
}