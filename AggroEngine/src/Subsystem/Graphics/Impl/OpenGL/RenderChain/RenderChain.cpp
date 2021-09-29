#include "RenderChain.hpp"

RenderChain::RenderChain()
{
}

int RenderChain::appendRenderData(shared_ptr<RenderData> renderData)
{
	boost::lock_guard<RenderChain> guard(*this);
	shared_ptr<RenderNode> newNode(new RenderNode(renderData));
	if (!m_first) {
		m_first = newNode;
	}
	if (m_last) {
		newNode->m_prev = m_last;
		m_last->m_next = newNode;
	}
	m_last = newNode;
	m_idToNode[newNode->getId()] = newNode;
	return newNode->getId();
}

bool RenderChain::removeRenderData(int nodeId)
{
	boost::lock_guard<RenderChain> guard(*this);
	if (m_idToNode.find(nodeId) != m_idToNode.end())
	{
		shared_ptr<RenderNode> prev = m_idToNode[nodeId]->m_prev;
		shared_ptr<RenderNode> next = m_idToNode[nodeId]->m_next;
		if (prev) {
			prev->m_next = next;
		} else {
			m_first = next;
		}
		if (next) {
			next->m_prev = prev;
		} else {
			m_last = prev;
		}
		m_idToNode.erase(nodeId);
		return true;
	}
	return false;
}

shared_ptr<RenderNode> RenderChain::getFirst()
{
	return m_first;
}

shared_ptr<RenderNode> RenderChain::getById(int nodeId)
{
	return m_idToNode[nodeId];
}