#pragma once

class ShadedBuffer;

#include "FrameBufferObject.hpp"
#include "OpenGL43Graphics.hpp"

/**
* Combines lighting and albedo to a final shaded scene
*
* author: wcrane
* since: 2017-12-10
**/
class ShadedBuffer : public FrameBufferObject
{
private:
	OpenGL43Graphics *m_graphics;
	shared_ptr<VertexBufferHandle> m_screenVBO;

public:
	ShadedBuffer(OpenGL43Graphics *graphics, int width, int height);

	void drawToBuffer(RenderOptions &renderOptions, shared_ptr<TextureHandle> albedoTex, shared_ptr<TextureHandle> lightingTex);
};