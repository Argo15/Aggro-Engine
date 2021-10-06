#include "MatrixLayer.hpp"
#include "MatrixCommand.hpp"

MatrixLayer::MatrixLayer(shared_ptr<GLSLProgram> glslProgram)
	: m_glslProgram(glslProgram)
{

}

shared_ptr<CommandTreeItem> MatrixLayer::getCommands(RenderOptions &renderOptions, vector<shared_ptr<RenderNode>> &renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());

	shared_ptr<glm::mat4> viewProj(new glm::mat4(renderOptions.getProjectionMatrix() * renderOptions.getViewMatrix()));

	for (int i = 0; i<renderNodes.size(); i++)
	{
		shared_ptr<Command> matCommand(new MatrixCommand(
			m_glslProgram,
			viewProj,
			&renderNodes[i]->getRenderData()->getModelMatrix()));

		item->addCommand(matCommand, renderNodes[i]);
	}
	return item;
}
