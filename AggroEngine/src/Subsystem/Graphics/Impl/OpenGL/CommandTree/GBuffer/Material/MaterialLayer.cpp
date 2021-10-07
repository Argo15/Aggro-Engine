#include "MaterialLayer.hpp"
#include "MaterialCommand.hpp"

static glm::mat4 defaultTexMatrix(1.0);
static glm::vec3 defaultColor(1.0);
static glm::vec3 defaultEmission(0);

MaterialLayer::MaterialLayer(shared_ptr<GLSLProgram> glslProgram)
	: m_glslProgram(glslProgram)
{

}

shared_ptr<CommandTreeItem> MaterialLayer::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());

	while (renderNodes)
	{
		shared_ptr<Material> material = renderNodes->getRenderData()->getMaterial();

		glm::mat4 *textureMatrix = material ? &material->getTextureMatrix() : &defaultTexMatrix;
		glm::vec3 *color = material ? &material->getColor() : &defaultColor;
		float specIntensity = material ? material->getSpecIntensity() : 0.f;
		float shininess = material ? (float)material->getShininess() : 0.f;
		glm::vec3 *emission = material ? &material->getEmission() : &defaultEmission;
		bool hasNormals = material && material->getNormalMapOpt().is_initialized();
		shared_ptr<glm::mat3> texRotateMatrix;
		if (hasNormals)
		{
			glm::mat3 rotMat = material->getTexRotateMatrix();
			rotMat[0] = glm::vec3(rotMat[0][0], -rotMat[0][1], 0);
			rotMat[1] = glm::vec3(-rotMat[1][0], rotMat[1][1], 0);
			rotMat[2] = glm::vec3(0, 0, 1.0);
			texRotateMatrix = shared_ptr<glm::mat3>(new glm::mat3(rotMat));
		}

		shared_ptr<Command> matCommand(new MaterialCommand(
			m_glslProgram,
			textureMatrix,
			texRotateMatrix,
			color, 
			specIntensity, 
			shininess, 
			emission, 
			hasNormals));

		item->addCommand(matCommand, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}
