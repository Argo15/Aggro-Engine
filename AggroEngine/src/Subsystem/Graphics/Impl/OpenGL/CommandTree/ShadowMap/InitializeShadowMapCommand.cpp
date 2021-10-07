#include "InitializeShadowMapCommand.hpp"
#include "Texture.hpp"

InitializeShadowMapCommand::InitializeShadowMapCommand(ShadowMapBuffer *shadowMapBuf)
	: Command()
	, m_shadowMapBuf(shadowMapBuf)
{
	m_hash = CommandType::INIT_SHADOW;
}

void InitializeShadowMapCommand::executeCommand()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_shadowMapBuf->getCurrentShadowBuffer());
	m_shadowMapBuf->getProgram()->use();
	GLenum mrt[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, m_shadowMapBuf->getWidth(), m_shadowMapBuf->getHeight());
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBindFragDataLocation(m_shadowMapBuf->getProgram()->getHandle(), 0, "testBuffer");
	glBindAttribLocation(m_shadowMapBuf->getProgram()->getHandle(), 0, "v_vertex");
	glEnableVertexAttribArray(0);
}

void InitializeShadowMapCommand::end()
{
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glUseProgram(0);
	glPopAttrib();
}

bool InitializeShadowMapCommand::equals(shared_ptr<Command> other)
{
	InitializeShadowMapCommand *cmd = static_cast<InitializeShadowMapCommand *>(other.get());
	return cmd->m_type == CommandType::INIT_SHADOW;
}