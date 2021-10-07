#include "MaterialCommand.hpp"

MaterialCommand::MaterialCommand(shared_ptr<GLSLProgram> glslProgram, glm::mat4 *textureMatrix, shared_ptr<glm::mat3> texRotateMatrix, glm::vec3 *color, float specIntensity, float shininess, glm::vec3 *emission, bool hasNormals)
	: m_glslProgram(glslProgram)
	, m_textureMatrix(textureMatrix)
	, m_texRotateMatrix(texRotateMatrix)
	, m_color(color)
	, m_specIntensity(specIntensity)
	, m_shininess(shininess)
	, m_emission(emission)
	, m_hasNormals(hasNormals)
{
	m_hash = CommandType::MATERIAL;
	m_hash = m_hash * 31 + (int)m_specIntensity;
	m_hash = m_hash * 31 + (int)m_shininess;
	m_hash = m_hash * 31 + (int)m_hasNormals;
}

void MaterialCommand::executeCommand()
{
	m_glslProgram->sendUniform("textureMatrix", glm::value_ptr(*m_textureMatrix), false, 4);
	m_glslProgram->sendUniform("material.color", *m_color);
	m_glslProgram->sendUniform("material.specIntensity", m_specIntensity);
	m_glslProgram->sendUniform("material.shininess", m_shininess);
	m_glslProgram->sendUniform("material.emission", *m_emission);
	m_glslProgram->sendUniform("material.hasNormals", m_hasNormals);
	if (m_texRotateMatrix)
	{
		m_glslProgram->sendUniform("texRotateMatrix", glm::value_ptr(*m_texRotateMatrix), false, 3);
	}
}

void MaterialCommand::end()
{

}

bool MaterialCommand::equals(shared_ptr<Command> other)
{
	MaterialCommand *cmd = static_cast<MaterialCommand *>(other.get());
	if (cmd->m_type != CommandType::MATERIAL)
	{
		return false;
	}

	if (m_texRotateMatrix && !cmd->m_texRotateMatrix) return false;
	if (!m_texRotateMatrix && cmd->m_texRotateMatrix) return false;

	return cmd->m_type == CommandType::MATERIAL &&
		*cmd->m_textureMatrix == *m_textureMatrix &&
		*cmd->m_color == *m_color &&
		cmd->m_specIntensity == m_specIntensity &&
		cmd->m_shininess == m_shininess &&
		*cmd->m_emission == *m_emission &&
		cmd->m_hasNormals == m_hasNormals &&
		((!m_texRotateMatrix && !cmd->m_texRotateMatrix) || m_texRotateMatrix.get() == cmd->m_texRotateMatrix.get());
}