#pragma once

class LightBuffer;

#include "FrameBufferObject.hpp"
#include "OpenGL43Graphics.hpp"

/**
* Uses normals and light info to calculate light contribution
*
* author: wcrane
* since: 2017-12-10
**/
class LightBuffer : public FrameBufferObject
{
private:
	OpenGL43Graphics *m_graphics;
	shared_ptr<TextureHandle> m_lightTexture;
	shared_ptr<TextureHandle> m_noLightTexture;
	shared_ptr<TextureHandle> m_glowTex;
	shared_ptr<VertexBufferHandle> m_screenVBO;

public:
	LightBuffer(OpenGL43Graphics *graphics, int width, int height);

	void drawToBuffer(RenderOptions &renderOptions, shared_ptr<TextureHandle> normalTex, shared_ptr<TextureHandle> depthTex, shared_ptr<TextureHandle> glowTex);

	shared_ptr<TextureHandle> getGlowTex();
};