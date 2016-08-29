#include "SceneNode.hpp"
#include "StringUtil.hpp"
#include <set>

SceneNode::SceneNode(SceneNode *parent, shared_ptr<Object> object)
	: m_parent(parent)
	, m_object(object)
	, m_children(shared_ptr<vector<shared_ptr<SceneNode>>>(new vector<shared_ptr<SceneNode>>()))
	, m_isSelected(false)
	, m_name("")
{
	setName("Object");
}

shared_ptr<Object> SceneNode::getObject()
{
	return m_object;
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
}

string SceneNode::getName()
{
	return m_name;
}

void SceneNode::setParent(SceneNode *parent)
{
	m_parent = parent;
}