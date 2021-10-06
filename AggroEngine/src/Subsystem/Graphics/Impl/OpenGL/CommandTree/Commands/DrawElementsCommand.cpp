#include "DrawElementsCommand.hpp"
#include "OpenGL43Graphics.hpp"

static glm::vec3 defaultColor(1.0);
static glm::vec3 defaultEmission(0);

DrawElementsCommand::DrawElementsCommand(shared_ptr<RenderData> renderData, shared_ptr<GLSLProgram> glslProgram)
	: Command()
	, m_renderData(renderData)
	, m_glslProgram(glslProgram)
{
}

void DrawElementsCommand::executeCommand()
{
	shared_ptr<Material> material = m_renderData->getMaterial();
	int texId = 0;
	if (material)
	{
		glm::mat4 textureMatrix = material->getTextureMatrix();
		m_glslProgram->sendUniform("textureMatrix", glm::value_ptr(textureMatrix), false, 4);

		m_glslProgram->sendUniform("material.color", material->getColor());
		m_glslProgram->sendUniform("material.specIntensity", material->getSpecIntensity());
		m_glslProgram->sendUniform("material.shininess", (float)material->getShininess());
		m_glslProgram->sendUniform("material.emission", material->getEmission());
		boost::optional<shared_ptr<TextureHandle>> normalMap = material->getNormalMapOpt();
		if (normalMap)
		{
			m_glslProgram->sendUniform("material.hasNormals", true);
			glm::mat3 texRotateMatrix = material->getTexRotateMatrix();
			texRotateMatrix[0] = glm::vec3(texRotateMatrix[0][0], -texRotateMatrix[0][1], 0);
			texRotateMatrix[1] = glm::vec3(-texRotateMatrix[1][0], texRotateMatrix[1][1], 0);
			texRotateMatrix[2] = glm::vec3(0, 0, 1.0);
			m_glslProgram->sendUniform("texRotateMatrix", glm::value_ptr(texRotateMatrix), false, 3);
		}
		else
		{
			m_glslProgram->sendUniform("material.hasNormals", false);
		}
	}
	else
	{
		m_glslProgram->sendUniform("material.color", defaultColor);
		m_glslProgram->sendUniform("material.specIntensity", 0.f);
		m_glslProgram->sendUniform("material.shininess", 0.f);
		m_glslProgram->sendUniform("material.emission", defaultEmission);
		m_glslProgram->sendUniform("material.hasNormals", false);
		glm::mat4 textureMatrix = glm::mat4(1.0);
		m_glslProgram->sendUniform("textureMatrix", glm::value_ptr(textureMatrix), false, 4);
	}

	unsigned int id = m_renderData->getId();
	float r = (id % 255) / 255.0f;
	float g = ((id / 255) % 255) / 255.0f;
	float b = ((id / 65025) % 255) / 255.0f;
	m_glslProgram->sendUniform("objId", r, g, b);
	m_glslProgram->sendUniform("lightingEnabled", m_renderData->isLightingEnabled());
	glLineWidth(m_renderData->getLineWidth());

	shared_ptr<VertexBufferHandle> vboHandle = m_renderData->getVertexBufferHandle();
	m_glslProgram->sendUniform("hasTangents", vboHandle->hasTangents());

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle->getVertexHandle());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle->getIndexHandle());

	if (vboHandle->hasTangents())
	{
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies()));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies() * 5 / 3));
	if (vboHandle->hasTangents())
	{
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies() * 8 / 3));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vboHandle->getSizeOfVerticies() * 11 / 3));
	}
	glDrawElements((GLenum)(m_renderData->getDrawMode()), vboHandle->getSizeOfIndicies(), GL_UNSIGNED_INT, 0);
}

void DrawElementsCommand::end()
{
	if (m_renderData->getVertexBufferHandle()->hasTangents())
	{
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
	}
}

bool DrawElementsCommand::equals(shared_ptr<Command> other)
{
	return false; // assume no duplicate draw commands
}