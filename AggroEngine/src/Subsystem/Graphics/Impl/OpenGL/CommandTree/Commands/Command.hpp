#pragma once

#include <vector>
#include <functional>
#include "RenderNode.hpp"
using namespace std;

enum CommandType {
	EMPTY = 0,
	INIT_GBUFFER = 1,
	DISABLE_DEPTH = 2,
	DRAW_ELEMENTS = 3,
	HAS_TANGENTS = 4,
	MATRIX = 5,
	TEXTURE = 6,
	MATERIAL = 7,
	INIT_SHADOW = 8,
	DRAW_SHADOW_ELEMENTS = 9,
	MATRIX_SHADOW = 10
};

class Command
{
protected:
	shared_ptr<RenderNode> m_renderNodes;
	int m_hash;

public:
	Command();

	void addRenderNode(shared_ptr<RenderNode> node);
	shared_ptr<RenderNode> getRenderNodes();

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other) { return false; };
	virtual int hash() { return m_hash; };
};
