#pragma once

#include "Command.hpp"
#include "Shaders/GLSLProgram.hpp"

class MVPMatrixCommand : public Command
{
private:
	CommandType m_type = CommandType::MATRIX_SHADOW;
	shared_ptr<GLSLProgram> m_glslProgram;
	glm::mat4 *m_viewProj;
	glm::mat4 m_modelMatrix;

public:
	MVPMatrixCommand(shared_ptr<GLSLProgram> glslProgram, glm::mat4 *viewProj, glm::mat4 &modelMatrix);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};
