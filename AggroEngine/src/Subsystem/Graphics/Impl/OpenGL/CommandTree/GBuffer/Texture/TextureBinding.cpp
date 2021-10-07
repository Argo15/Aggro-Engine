#include "TextureBinding.hpp"
#include "TextureBindingCommand.hpp"

TextureBinding::TextureBinding(shared_ptr<GLSLProgram> glslProgram, shared_ptr<BufferSyncContext> syncContext, shared_ptr<TextureHandle> whiteTexture)
	: m_glslProgram(glslProgram)
	, m_syncContext(syncContext)
	, m_whiteTexture(whiteTexture)
{

}

shared_ptr<CommandTreeItem> TextureBinding::getCommands(RenderOptions &renderOptions, shared_ptr<RenderNode> renderNodes)
{
	shared_ptr<CommandTreeItem> item(new CommandTreeItem());
	while (renderNodes)
	{
		shared_ptr<Material> material = renderNodes->getRenderData()->getMaterial();

		shared_ptr<TextureHandle> albedo = material ? material->getTextureOpt().get_value_or(m_whiteTexture) : m_whiteTexture;
		shared_ptr<TextureHandle> alpha = material ? material->getAlphaOpt().get_value_or(m_whiteTexture) : m_whiteTexture;
		shared_ptr<TextureHandle> specular = material ? material->getSpecularOpt().get_value_or(m_whiteTexture) : m_whiteTexture;
		shared_ptr<TextureHandle> emission = material ? material->getEmissionMapOpt().get_value_or(m_whiteTexture) : m_whiteTexture;
		shared_ptr<TextureHandle> normal = material ? material->getNormalMapOpt().get_value_or(shared_ptr<TextureHandle>()) : shared_ptr<TextureHandle>();

		if (!m_syncContext->checkAndClearSync(albedo->get()))   albedo = m_whiteTexture;
		if (!m_syncContext->checkAndClearSync(alpha->get()))    alpha = m_whiteTexture;
		if (!m_syncContext->checkAndClearSync(specular->get())) specular = m_whiteTexture;
		if (!m_syncContext->checkAndClearSync(emission->get())) emission = m_whiteTexture;
		if (normal && !m_syncContext->checkAndClearSync(normal->get()))   normal = m_whiteTexture;

		shared_ptr<Command> texCommand(new TextureBindingCommand(
			m_glslProgram,
			albedo,
			alpha,
			specular,
			emission,
			normal));

		item->addCommand(texCommand, renderNodes);
		renderNodes = renderNodes->next();
	}
	return item;
}
