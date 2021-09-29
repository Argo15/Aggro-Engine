#include "RenderNode.hpp"

RenderNode::RenderNode(shared_ptr<RenderData> renderData)
	: m_renderData(renderData)
	, m_id(getNextId())
	, m_prev()
	, m_next()
{
}

int RenderNode::getId()
{
	return m_id;
}

shared_ptr<RenderData> RenderNode::getRenderData()
{
	return m_renderData;
}

shared_ptr<RenderNode> RenderNode::next()
{
	return m_next;
}

int RenderNode::s_nextId = 1;
unsigned int RenderNode::getNextId()
{
	return s_nextId++;
}