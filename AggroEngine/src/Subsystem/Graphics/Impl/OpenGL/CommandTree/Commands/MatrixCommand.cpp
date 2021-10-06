#include "MatrixCommand.hpp"

MatrixCommand::MatrixCommand(shared_ptr<GLSLProgram> glslProgram, shared_ptr<glm::mat4> viewProj, glm::mat4 *modelMatrix)
	: m_glslProgram(glslProgram)
	, m_viewProj(viewProj)
	, m_modelMatrix(modelMatrix)
{

}

void MatrixCommand::executeCommand()
{
	glm::mat4 mvpMatrix = *m_viewProj * *m_modelMatrix;
	m_glslProgram->sendUniform("modelViewProjectionMatrix", glm::value_ptr(mvpMatrix), false, 4);
	glm::mat3 normalMatrix = glm::mat3(*m_modelMatrix);
	m_glslProgram->sendUniform("normalMatrix", glm::value_ptr(normalMatrix), false, 3);
}

void MatrixCommand::end()
{

}

bool MatrixCommand::equals(shared_ptr<Command> other)
{
	MatrixCommand *cmd = static_cast<MatrixCommand *>(other.get());
	return cmd->m_type == CommandType::MATRIX &&
		*cmd->m_modelMatrix == *m_modelMatrix;
}