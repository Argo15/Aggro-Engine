#pragma once

class OpenGL43Graphics;

#include <queue>
#include <memory>
#include "Shaders/ShaderStore.hpp"
#include "Framebuffer/GBuffer.hpp"
#include "Graphics.hpp"
#include "Locks.hpp"

/**
 * OpenGL graphics that only guarentees support for OpenGL 4.3 or above.
 *
 * author: wcrane
 * since: 2013-11-03
**/
class OpenGL43Graphics : public Graphics, 
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	std::queue<shared_ptr<RenderData>> renderQueue;
	ShaderStore m_shaderStore;
	shared_ptr<GBuffer> m_gBuffer;
	shared_ptr<Viewport> m_viewport;

	void _drawScreen(RenderOptions &renderOptions, float nX1, float nY1, float nX2, float nY2);
	shared_ptr<TextureHandle> _getRenderTargetTexture(RenderOptions::RenderTarget target);

public:
	OpenGL43Graphics();
	~OpenGL43Graphics();

	void init();

	shared_ptr<VertexBufferHandle> createVertexBuffer(shared_ptr<Mesh> mesh); 
	void deleteVertexBuffer(shared_ptr<VertexBufferHandle> nHandle);
	
	shared_ptr<TextureHandle> createTexture();
	shared_ptr<TextureHandle> createTexture(shared_ptr<Image> image);
	shared_ptr<TextureHandle> createTexture(shared_ptr<TextureBuildOptions> pTexOptions);
	void deleteTexture(shared_ptr<TextureHandle> textureHandle);

	void stageTriangleRender(shared_ptr<RenderData> pRenderData);
	void executeRender(RenderOptions &renderOptions);

	void setViewport(int nX, int nY, int nWidth, int nHeight);
	shared_ptr<Viewport> getViewport();
	void clearColor();
	void clearDepth();
	void clearDepthAndColor();

	ShaderStore getShaderStore();

	shared_ptr<Image> getRenderImage(RenderOptions::RenderTarget target);
	shared_ptr<Image> getRenderImage(int x, int y, int width, int height, RenderOptions::RenderTarget target);
	boost::shared_array<unsigned short> getRenderImagePixel(int x, int y, RenderOptions::RenderTarget target);

	int getFrameBufferWidth();
	int getFrameBufferHeight();
};