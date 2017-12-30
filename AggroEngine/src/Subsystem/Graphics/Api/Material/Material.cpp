#include "Material.hpp"

Material::Material(glm::vec3 color)
	: m_color(color)
	, m_specIntensity(0)
	, m_shininess(25)
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

void Material::setSpecIntensity(float intensity)
{
	m_specIntensity = intensity;
}

float Material::getSpecIntensity()
{
	return m_specIntensity;
}

void Material::setShininess(int shininess)
{
	m_shininess = shininess;
}

int Material::getShininess()
{
	return m_shininess;
}

void Material::setSpecular(shared_ptr<TextureHandle> texture)
{
	m_specular = texture;
}

shared_ptr<TextureHandle> Material::getSpecular()
{
	return m_specular;
}

boost::optional<shared_ptr<TextureHandle>> Material::getSpecularOpt()
{
	if (m_specular)
	{
		return boost::optional<shared_ptr<TextureHandle>>(m_specular);
	}
	return boost::optional<shared_ptr<TextureHandle>>();
}