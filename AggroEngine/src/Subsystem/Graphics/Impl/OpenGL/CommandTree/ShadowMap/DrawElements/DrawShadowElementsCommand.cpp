#include "DrawShadowElementsCommand.hpp"
#include "OpenGL43Graphics.hpp"

DrawShadowElementsCommand::DrawShadowElementsCommand(shared_ptr<RenderData> renderData)
	: Command()
	, m_renderData(renderData)
{
}

void DrawShadowElementsCommand::executeCommand()
{
	shared_ptr<VertexBufferHandle> vboHandle = m_renderData->getVertexBufferHandle();
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle->getVertexHandle());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle->getIndexHandle());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements((GLenum)(m_renderData->getDrawMode()), vboHandle->getSizeOfIndicies(), GL_UNSIGNED_INT, 0);
}

void DrawShadowElementsCommand::end()
{
}

bool DrawShadowElementsCommand::equals(shared_ptr<Command> other)
{
	return false; // assume no duplicate draw commands
}