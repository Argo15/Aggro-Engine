#pragma once

#include "Layer.hpp"
#include "BufferSyncContext.hpp"
#include "Shaders/GLSLProgram.hpp"

class DrawElements : public Layer
{
private:
	shared_ptr<BufferSyncContext> m_syncContext;
	shared_ptr<GLSLProgram> m_glslProgram;
	shared_ptr<TextureHandle> m_whiteTexture;

public:
	DrawElements(shared_ptr<BufferSyncContext> syncContext, shared_ptr<GLSLProgram> m_glslProgram, shared_ptr<TextureHandle> whiteTexture);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes);
};