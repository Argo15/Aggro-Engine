#pragma once

#include "Layer.hpp"
#include "Shaders/GLSLProgram.hpp"
#include "BufferSyncContext.hpp"

class MaterialLayer : public Layer
{
private:
	shared_ptr<GLSLProgram> m_glslProgram;

public:
	MaterialLayer(shared_ptr<GLSLProgram> glslProgram);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes);
};