#pragma once

#include <vector>
#include "RenderNode.hpp"
using namespace std;

enum CommandType {
	EMPTY = 0,
	INIT_GBUFFER = 1,
	DISABLE_DEPTH = 2,
	DRAW_ELEMENTS = 3,
	HAS_TANGENTS = 4,
	MATRIX = 5,
	TEXTURE = 6
};

class Command
{
protected:
	vector<shared_ptr<RenderNode>> m_renderNodes;

public:
	Command();

	void addRenderNode(shared_ptr<RenderNode> node);
	vector<shared_ptr<RenderNode>> &getRenderNodes();

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other) { return false; };
};
