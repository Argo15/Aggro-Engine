#pragma once

#include "Command.hpp"
#include "Shaders/GLSLProgram.hpp"

class MVPMatrixCommand : public Command
{
private:
	CommandType m_type = CommandType::MATRIX_SHADOW;
	shared_ptr<GLSLProgram> m_glslProgram;
	shared_ptr<glm::mat4> m_mvpMatrix;
	float *m_mvpMatrixVals;

public:
	MVPMatrixCommand(shared_ptr<GLSLProgram> glslProgram, shared_ptr<glm::mat4> mvpMatrix);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};
