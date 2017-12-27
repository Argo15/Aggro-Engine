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