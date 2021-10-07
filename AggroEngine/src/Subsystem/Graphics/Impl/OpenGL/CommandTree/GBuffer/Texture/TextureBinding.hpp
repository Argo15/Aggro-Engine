#pragma once

#include "Layer.hpp"
#include "Shaders/GLSLProgram.hpp"
#include "BufferSyncContext.hpp"

class TextureBinding : public Layer
{
private:
	shared_ptr<GLSLProgram> m_glslProgram;
	shared_ptr<BufferSyncContext> m_syncContext;
	shared_ptr<TextureHandle> m_whiteTexture;

public:
	TextureBinding(shared_ptr<GLSLProgram> glslProgram, shared_ptr<BufferSyncContext> syncContext, shared_ptr<TextureHandle> whiteTexture);

	shared_ptr<CommandTreeItem> getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes);
};