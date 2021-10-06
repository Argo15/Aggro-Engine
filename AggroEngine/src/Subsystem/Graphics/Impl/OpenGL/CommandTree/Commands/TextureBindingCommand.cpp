#include "TextureBindingCommand.hpp"


TextureBindingCommand::TextureBindingCommand(shared_ptr<GLSLProgram> glslProgram,
	shared_ptr<TextureHandle> albedo,
	shared_ptr<TextureHandle> alpha,
	shared_ptr<TextureHandle> specular,
	shared_ptr<TextureHandle> emission,
	shared_ptr<TextureHandle> normal)
	: Command()
	, m_glslProgram(glslProgram)
	, m_albedo(albedo)
	, m_alpha(alpha)
	, m_specular(specular)
	, m_emission(emission)
	, m_normal(normal)
{
}

void TextureBindingCommand::executeCommand()
{
	int texId = 0;
	m_glslProgram->sendUniform("material.tex", m_albedo, texId++);
	m_glslProgram->sendUniform("material.alpha", m_alpha, texId++);
	m_glslProgram->sendUniform("material.specMap", m_specular, texId++);
	m_glslProgram->sendUniform("material.emissionMap", m_emission, texId++);
	if (m_normal)
	{
		m_glslProgram->sendUniform("material.normalMap", m_normal, texId++);
	}
}

void TextureBindingCommand::end()
{
}

bool TextureBindingCommand::equals(shared_ptr<Command> other)
{
	TextureBindingCommand *cmd = static_cast<TextureBindingCommand *>(other.get());
	if (m_normal && !cmd->m_normal) return false;
	if (!m_normal && cmd->m_normal) return false;

	return cmd->m_type == CommandType::TEXTURE &&
		m_albedo->get() == cmd->m_albedo->get() &&
		m_alpha->get() == cmd->m_alpha->get() &&
		m_specular->get() == cmd->m_specular->get() &&
		m_emission->get() == cmd->m_emission->get() &&
		((!m_normal && !cmd->m_normal) || m_normal->get() == cmd->m_normal->get());
}