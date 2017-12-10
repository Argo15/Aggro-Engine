#pragma once

class GBuffer;
class FrameBufferObject;

#include "FrameBufferObject.hpp"
#include "OpenGL43Graphics.hpp"

class GBuffer : public FrameBufferObject
{
private:
	shared_ptr<TextureHandle> m_depthTex;
	shared_ptr<TextureHandle> m_normalTex;
	shared_ptr<TextureHandle> m_albedoTex;
	shared_ptr<TextureHandle> m_selectionTex;
	OpenGL43Graphics *m_graphics;

public:
	GBuffer(OpenGL43Graphics *graphics, int width, int height);

	void drawToBuffer(RenderOptions renderOptions, std::queue<shared_ptr<RenderData>> &renderQueue);

	void bindDepthTex();
	void bindNormalTex();
	void bindAlbedoTex();
	void bindSelectionTex();

	shared_ptr<TextureHandle> getDepthTex();
	shared_ptr<TextureHandle> getNormalTex();
	shared_ptr<TextureHandle> getAlbedoTex();
	shared_ptr<TextureHandle> getSelectionTex();

	GLenum getSelectionColorAttachment();
};