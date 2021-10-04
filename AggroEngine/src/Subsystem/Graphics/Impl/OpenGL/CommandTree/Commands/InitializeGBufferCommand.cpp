#include "InitializeGBufferCommand.hpp"

InitializeGBufferCommand::InitializeGBufferCommand(GLuint framebuffer, GLuint program, GLsizei width, GLsizei height)
	: Command()
	, m_framebuffer(framebuffer)
	, m_program(program)
	, m_width(width)
	, m_height(height)
{
}

void InitializeGBufferCommand::executeCommand()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
	glUseProgram(m_program);
	GLenum mrt[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
	glDrawBuffers(4, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, m_width, m_height);
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glBindFragDataLocation(m_program, 0, "normalBuffer");
	glBindFragDataLocation(m_program, 1, "albedoBuffer");
	glBindFragDataLocation(m_program, 2, "selectionBuffer");
	glBindFragDataLocation(m_program, 3, "glowBuffer");
	glBindAttribLocation(m_program, 0, "v_vertex");
	glBindAttribLocation(m_program, 1, "v_texcoord");
	glBindAttribLocation(m_program, 2, "v_normal");
	glBindAttribLocation(m_program, 3, "v_tangent");
	glBindAttribLocation(m_program, 4, "v_bitangent");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void InitializeGBufferCommand::end()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glUseProgram(0);
	glPopAttrib();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

bool InitializeGBufferCommand::equals(shared_ptr<Command> other)
{
	InitializeGBufferCommand *cmd = static_cast<InitializeGBufferCommand *>(other.get());
	return cmd->m_type == CommandType::INIT_GBUFFER;
}