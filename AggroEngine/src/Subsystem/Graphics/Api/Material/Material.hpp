#pragma once

#include "Matrix.hpp"
#include "TextureHandle.hpp"
#include <memory>
using namespace std;

class Material
{
private:
	glm::vec3 m_color;
	shared_ptr<TextureHandle> m_texture;

public:
	Material(glm::vec3 color);

	glm::vec3 &getColor();

	void setTexture(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getTexture();
};