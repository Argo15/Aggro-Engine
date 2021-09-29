#pragma once

#include <memory>
#include "RenderData.hpp"

/*
 * Node in the Render Chain. Pointers to adjecent are set in the RenderChain class
 */
class RenderNode 
{
friend class RenderChain;
private:
	int m_id;
	shared_ptr<RenderData> m_renderData;
	shared_ptr<RenderNode> m_prev;
	shared_ptr<RenderNode> m_next;

	static int s_nextId;

public:
	RenderNode(shared_ptr<RenderData> renderData);

	int getId();
	shared_ptr<RenderData> getRenderData();
	shared_ptr<RenderNode> next();

	static unsigned int getNextId();
};