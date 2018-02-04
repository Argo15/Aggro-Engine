#include "Material.hpp"

Material::Material(glm::vec3 color)
	: m_color(color)
	, m_specIntensity(0)
	, m_shininess(25)
	, m_textureMatrix(glm::mat4(1.0))
	, m_texRotateMatrix(glm::mat4(1.0))
	, m_emissionColor(0)
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

void Material::setTextureMatrix(glm::mat4 textureMatrix)
{
	m_textureMatrix = textureMatrix;
}

glm::mat4 Material::getTextureMatrix()
{
	return m_textureMatrix;
}

void Material::setTexRotateMatrix(glm::mat4 texRotateMatrix)
{
	m_texRotateMatrix = texRotateMatrix;
}

glm::mat4 Material::getTexRotateMatrix()
{
	return m_texRotateMatrix;
}

void Material::setEmission(glm::vec3 emission)
{
	m_emissionColor = emission;
}

glm::vec3 &Material::getEmission()
{
	return m_emissionColor;
}

void Material::setEmissionMap(shared_ptr<TextureHandle> texture)
{
	m_emissionMap = texture;
}

shared_ptr<TextureHandle> Material::getEmissionMap()
{
	return m_emissionMap;
}

boost::optional<shared_ptr<TextureHandle>> Material::getEmissionMapOpt()
{
	if (m_emissionMap)
	{
		return boost::optional<shared_ptr<TextureHandle>>(m_emissionMap);
	}
	return boost::optional<shared_ptr<TextureHandle>>();
}

void Material::setNormalMap(shared_ptr<TextureHandle> texture)
{
	m_normalMap = texture;
}

shared_ptr<TextureHandle> Material::getNormalMap()
{
	return m_normalMap;
}

boost::optional<shared_ptr<TextureHandle>> Material::getNormalMapOpt()
{
	if (m_normalMap)
	{
		return boost::optional<shared_ptr<TextureHandle>>(m_normalMap);
	}
	return boost::optional<shared_ptr<TextureHandle>>();
}