#include "Material.hpp"

Material::Material(glm::vec3 color)
	: m_color(color)
{
}

glm::vec3 &Material::getColor()
{
	return m_color;
}

void Material::setTexture(shared_ptr<TextureHandle> texture)
{
	m_texture = texture;
}

shared_ptr<TextureHandle> Material::getTexture()
{
	return m_texture;
}

boost::optional<shared_ptr<TextureHandle>> Material::getTextureOpt()
{
	if (m_texture)
	{
		return boost::optional<shared_ptr<TextureHandle>>(m_texture);
	}
	return boost::optional<shared_ptr<TextureHandle>>();
}

void Material::setAlpha(shared_ptr<TextureHandle> texture)
{
	m_alpha = texture;
}

shared_ptr<TextureHandle> Material::getAlpha()
{
	return m_alpha;
}

boost::optional<shared_ptr<TextureHandle>> Material::getAlphaOpt()
{
	if (m_alpha)
	{
		return boost::optional<shared_ptr<TextureHandle>>(m_alpha);
	}
	return boost::optional<shared_ptr<TextureHandle>>();
}