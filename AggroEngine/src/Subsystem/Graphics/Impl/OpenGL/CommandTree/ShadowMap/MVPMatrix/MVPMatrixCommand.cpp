#include "MVPMatrixCommand.hpp"
#include "OpenGL43Graphics.hpp"

MVPMatrixCommand::MVPMatrixCommand(shared_ptr<GLSLProgram> glslProgram, glm::mat4 *viewProj, glm::mat4 &modelMatrix)
	: Command()
	, m_glslProgram(glslProgram)
	, m_viewProj(viewProj)
	, m_modelMatrix(modelMatrix)
{
}

void MVPMatrixCommand::executeCommand()
{
	glm::mat4 mvp = (*m_viewProj) * (m_modelMatrix);
	m_glslProgram->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvp), false, 4);
}

void MVPMatrixCommand::end()
{
}

bool MVPMatrixCommand::equals(shared_ptr<Command> other)
{
	MVPMatrixCommand *cmd = static_cast<MVPMatrixCommand *>(other.get());
	return cmd->m_type == CommandType::MATRIX_SHADOW &&
		*cmd->m_viewProj == *m_viewProj &&
		cmd->m_modelMatrix == m_modelMatrix;
}