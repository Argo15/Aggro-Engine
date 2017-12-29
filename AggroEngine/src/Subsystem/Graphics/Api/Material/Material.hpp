#pragma once

#include "Matrix.hpp"
#include "TextureHandle.hpp"
#include <memory>
#include <boost/optional.hpp>
using namespace std;

class Material
{
private:
	glm::vec3 m_color;
	shared_ptr<TextureHandle> m_texture;
	shared_ptr<TextureHandle> m_alpha;

public:
	Material(glm::vec3 color);

	glm::vec3 &getColor();

	void setTexture(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getTexture();
	boost::optional<shared_ptr<TextureHandle>> getTextureOpt();

	void setAlpha(shared_ptr<TextureHandle> texture);
	shared_ptr<TextureHandle> getAlpha();
	boost::optional<shared_ptr<TextureHandle>> getAlphaOpt();
};