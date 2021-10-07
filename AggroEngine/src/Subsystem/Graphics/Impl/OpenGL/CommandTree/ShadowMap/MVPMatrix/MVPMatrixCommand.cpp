#include "MVPMatrixCommand.hpp"
#include "OpenGL43Graphics.hpp"

MVPMatrixCommand::MVPMatrixCommand(shared_ptr<GLSLProgram> glslProgram, shared_ptr<glm::mat4> mvpMatrix)
	: Command()
	, m_glslProgram(glslProgram)
	, m_mvpMatrix(mvpMatrix)
	, m_mvpMatrixVals(glm::value_ptr(*mvpMatrix))
{
}

void MVPMatrixCommand::executeCommand()
{
	m_glslProgram->sendUniform("modelViewProjectionMatrix", m_mvpMatrixVals, false, 4);
}

void MVPMatrixCommand::end()
{
}

bool MVPMatrixCommand::equals(shared_ptr<Command> other)
{
	MVPMatrixCommand *cmd = static_cast<MVPMatrixCommand *>(other.get());
	return cmd->m_type == CommandType::MATRIX_SHADOW &&
		*cmd->m_mvpMatrix == *m_mvpMatrix;
}