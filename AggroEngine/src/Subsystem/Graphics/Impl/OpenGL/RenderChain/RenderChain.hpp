#pragma once

#include <memory>
#include <boost/unordered_map.hpp>
#include "RenderNode.hpp"
#include "Locks.hpp"

class RenderChain : 
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<RenderNode> m_first;
	shared_ptr<RenderNode> m_last;
	boost::unordered_map<int, shared_ptr<RenderNode>> m_idToNode;

public:
	RenderChain();

	int appendRenderData(shared_ptr<RenderData> renderData); // Return nodeId
	bool removeRenderData(int nodeId);

	shared_ptr<RenderNode> getFirst();
	shared_ptr<RenderNode> getById(int nodeId);

	int size();
};