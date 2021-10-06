#pragma once

#include "Command.hpp"
#include "BufferSyncContext.hpp"
#include "Shaders/GLSLProgram.hpp"

class DrawElementsCommand : public Command
{
private:
	CommandType m_type = CommandType::DRAW_ELEMENTS;
	shared_ptr<RenderData> m_renderData;
	shared_ptr<GLSLProgram> m_glslProgram;

public:
	DrawElementsCommand(shared_ptr<RenderData> renderData, shared_ptr<GLSLProgram> glslProgram);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};
