#pragma once

#include <vector>
#include "Command.hpp"
#include "GBuffer.hpp"
using namespace std;

class InitializeGBufferCommand : public Command
{
private:
	CommandType m_type = CommandType::INIT_GBUFFER;
	GLuint m_framebuffer;
	GLuint m_program;
	GLsizei m_width;
	GLsizei m_height;

public:
	InitializeGBufferCommand(GLuint framebuffer, GLuint program, GLsizei width, GLsizei height);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};
