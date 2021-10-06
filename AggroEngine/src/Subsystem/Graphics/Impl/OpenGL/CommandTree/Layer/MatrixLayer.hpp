#pragma once

#include "Layer.hpp"
#include "Shaders/GLSLProgram.hpp"

class MatrixLayer : public Layer
{
private:
	shared_ptr<GLSLProgram> m_glslProgram;

public:
	MatrixLayer(shared_ptr<GLSLProgram> glslProgram);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes);
};