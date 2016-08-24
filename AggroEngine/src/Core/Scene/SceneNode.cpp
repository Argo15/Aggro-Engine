#include "SceneNode.hpp"

SceneNode::SceneNode(shared_ptr<Object> object)
	: m_object(object)
	, m_children(shared_ptr<vector<shared_ptr<SceneNode>>>(new vector<shared_ptr<SceneNode>>()))
{
}

shared_ptr<Object> SceneNode::getObject()
{
	return m_object;
}

shared_ptr<vector<shared_ptr<SceneNode>>> SceneNode::getChildren()
{
	return m_children;
}

void SceneNode::addChild(shared_ptr<SceneNode> child)
{
	m_children->push_back(child);
}