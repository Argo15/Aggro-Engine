#include "DrawElements.hpp"
#include "DrawElementsCommand.hpp"

DrawElements::DrawElements(shared_ptr<BufferSyncContext> syncContext, shared_ptr<GLSLProgram> glslProgram, shared_ptr<TextureHandle> whiteTexture)
	: m_syncContext(syncContext)
	, m_glslProgram(glslProgram)
	, m_whiteTexture(whiteTexture)
{

}

shared_ptr<CommandTreeItem> DrawElements::getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());

	glm::mat4 viewProj = renderOptions.getProjectionMatrix() * renderOptions.getViewMatrix();

	for (int i = 0; i<renderNodes.size(); i++)
	{
		shared_ptr<Command> drawCommand(new DrawElementsCommand(
			renderNodes[i]->getRenderData(),
			viewProj,
			m_syncContext,
			m_glslProgram,
			m_whiteTexture));

		item->addCommand(drawCommand, renderNodes[i]);
	}
	return item;
}