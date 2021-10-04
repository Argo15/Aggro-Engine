#pragma once

#include "Command.hpp"
#include "BufferSyncContext.hpp"
#include "Shaders/GLSLProgram.hpp"

class DrawElementsCommand : public Command
{
private:
	CommandType m_type = CommandType::DRAW_ELEMENTS;
	shared_ptr<RenderData> m_renderData;
	glm::mat4 m_viewProj;
	shared_ptr<BufferSyncContext> m_syncContext;
	shared_ptr<GLSLProgram> m_glslProgram;
	shared_ptr<TextureHandle> m_whiteTexture;

public:
	DrawElementsCommand(shared_ptr<RenderData> renderData, glm::mat4 viewProj, 
		shared_ptr<BufferSyncContext> syncContext, shared_ptr<GLSLProgram> glslProgram,
		shared_ptr<TextureHandle> whiteTexture);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};
