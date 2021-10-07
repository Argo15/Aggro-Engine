#include "MVPMatriX.hpp"
#include "MVPMatrixCommand.hpp"

MVPMatrix::MVPMatrix(ShadowMapBuffer *shadowMapBuf)
	: m_shadowMapBuf(shadowMapBuf)
{

}

shared_ptr<CommandTreeItem> MVPMatrix::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	glm::mat4 *viewProj = m_shadowMapBuf->getViewProjection(m_shadowMapBuf->currentSlice());

	while (renderNodes)
	{
		glm::mat4 modelMat = renderNodes->getRenderData()->getModelMatrix();
		shared_ptr<Command> drawCommand(new MVPMatrixCommand(
			m_shadowMapBuf->getProgram(), 
			viewProj,
			modelMat));

		item->addCommand(drawCommand, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}