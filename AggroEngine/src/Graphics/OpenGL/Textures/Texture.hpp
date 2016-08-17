#pragma once

#include "../../Graphics.hpp"

/**
 * OpenGL texture.
 *
 * author: wcrane
 * since: 2014-01-04
**/
class Texture
{
protected:
	GLuint m_nHandle;

public:
	Texture();

	void bind();
	void remove();
	static void unbind();
	shared_ptr<TextureHandle> getHandle();
};