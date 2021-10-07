#pragma once

#include "Command.hpp"
#include "Shaders/GLSLProgram.hpp"

class MatrixCommand : public Command
{
private:
	CommandType m_type = CommandType::MATRIX;
	shared_ptr<GLSLProgram> m_glslProgram;
	shared_ptr<glm::mat4> m_viewProj;
	glm::mat4 *m_modelMatrix;

public:
	MatrixCommand(shared_ptr<GLSLProgram> glslProgram, shared_ptr<glm::mat4> viewProj, glm::mat4 *modelMatrix);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};
