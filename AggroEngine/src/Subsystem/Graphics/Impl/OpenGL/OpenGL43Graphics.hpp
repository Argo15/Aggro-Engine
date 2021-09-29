#pragma once

class OpenGL43Graphics;

#include <queue>
#include <memory>
#include "Shaders/ShaderStore.hpp"
#include "GBuffer.hpp"
#include "LightBuffer.hpp"
#include "ShadedBuffer.hpp"
#include "Graphics.hpp"
#include "Locks.hpp"
#include "ShadowMapBuffer.hpp"
#include "MemoryUtil.hpp"
#include "Sync/BufferSyncContext.hpp"
#include "PixelBuffer/PixelBufferCache.hpp"
#include "RenderChain/RenderChain.hpp"

class PixelBufferCache;

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
	ShaderStore m_shaderStore;
	shared_ptr<ShadowMapBuffer> m_shadowBuffer;
	shared_ptr<GBuffer> m_gBuffer;
	shared_ptr<LightBuffer> m_lightBuffer;
	shared_ptr<ShadedBuffer> m_shadedBuffer;
	shared_ptr<Viewport> m_viewport;
	shared_ptr<GLSLProgram> m_screenProgram;
	shared_ptr<VertexBufferHandle> m_screenVBO;
	shared_ptr<BufferSyncContext> m_syncContext;
	shared_ptr<PixelBufferCache> m_pixelBuffers;
	shared_ptr<RenderChain> m_renderChain;

	void _drawScreen(RenderOptions &renderOptions, float nX1, float nY1, float nX2, float nY2);
	shared_ptr<TextureHandle> _getRenderTargetTexture(RenderOptions::RenderTarget target);

public:
	OpenGL43Graphics();
	~OpenGL43Graphics();

	void init(shared_ptr<GraphicsInitOptions> options);

	shared_ptr<VertexBufferHandle> createVertexBuffer(shared_ptr<Mesh> mesh); 
	void deleteVertexBuffer(shared_ptr<VertexBufferHandle> nHandle);
	
	shared_ptr<TextureHandle> createTexture();
	shared_ptr<TextureHandle> createTexture(shared_ptr<ImageUC> image);
	shared_ptr<TextureHandle> createTexture(shared_ptr<TextureBuildOptions> pTexOptions);
	shared_ptr<TextureHandle> createTextureAsync(shared_ptr<TextureBuildOptions> texOptions);
	void deleteTexture(shared_ptr<TextureHandle> textureHandle);

	shared_ptr<RenderHandle> stageRender(shared_ptr<RenderData> renderData);
	void executeRender(RenderOptions &renderOptions);
	void drawScreen(RenderOptions &renderOptions);

	void setViewport(int nX, int nY, int nWidth, int nHeight);
	shared_ptr<Viewport> getViewport();
	void clearColor(int frameBufferId);
	void clearDepth(int frameBufferId);
	void clearDepthAndColor(int frameBufferId);

	ShaderStore getShaderStore();

	shared_ptr<ImageUC> getSelectionImage(int x, int y, int width, int height);
	shared_ptr<ImageF> getDepthImage(int x, int y, int width, int height);
	shared_ptr<unsigned char> getSelectionImagePixel(int x, int y);
	shared_ptr<float> getDepthImagePixel(int x, int y);

	int getFrameBufferWidth();
	int getFrameBufferHeight();
};