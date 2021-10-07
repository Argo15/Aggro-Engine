#pragma once

#include "Layer.hpp"
#include "BufferSyncContext.hpp"
#include "Shaders/GLSLProgram.hpp"

class DrawElements : public Layer
{
private:
	shared_ptr<GLSLProgram> m_glslProgram;

public:
	DrawElements(shared_ptr<GLSLProgram> glslProgram);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};