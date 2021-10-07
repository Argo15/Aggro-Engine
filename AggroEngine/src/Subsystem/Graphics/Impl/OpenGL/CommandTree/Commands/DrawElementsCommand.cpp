#include "DrawElementsCommand.hpp"
#include "OpenGL43Graphics.hpp"

DrawElementsCommand::DrawElementsCommand(shared_ptr<RenderData> renderData, shared_ptr<GLSLProgram> glslProgram)
	: Command()
	, m_renderData(renderData)
	, m_glslProgram(glslProgram)
{
}

void DrawElementsCommand::executeCommand()
{
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