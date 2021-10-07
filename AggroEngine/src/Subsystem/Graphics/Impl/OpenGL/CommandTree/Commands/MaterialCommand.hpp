#pragma once

#include "Command.hpp"
#include "Shaders/GLSLProgram.hpp"

class MaterialCommand : public Command
{
private:
	CommandType m_type = CommandType::MATERIAL;
	shared_ptr<GLSLProgram> m_glslProgram;
	glm::mat4 *m_textureMatrix;
	shared_ptr<glm::mat3> m_texRotateMatrix;
	glm::vec3 *m_color;
	float m_specIntensity;
	float m_shininess;
	glm::vec3 *m_emission;
	bool m_hasNormals;

public:
	MaterialCommand(shared_ptr<GLSLProgram> glslProgram, glm::mat4 *textureMatrix, shared_ptr<glm::mat3> texRotateMatrix, glm::vec3 *color, float specIntensity, float shininess, glm::vec3 *emission, bool hasNormals);

	virtual void executeCommand();
	virtual void end();
	virtual bool equals(shared_ptr<Command> other);
};
