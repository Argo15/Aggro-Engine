#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <memory>
#include "VertexBufferHandle.hpp"
#include "RenderData.hpp"
#include "Mesh.hpp"
#include "Image.hpp"
#include "RenderOptions.hpp"
#include "TextureHandle.hpp"
#include "TextureBuildOptions.hpp"
#include "Viewport.hpp"
using namespace std;

/**
 * Base class for graphics APIs
 *
 * author: wcrane
 * since: 2013-11-03
**/
class Graphics
{
public:
	virtual ~Graphics() {}

	virtual void init() = 0;

	// Vertex buffers
	// Returns a vertex buffer handle
	virtual shared_ptr<VertexBufferHandle> createVertexBuffer(shared_ptr<Mesh> mesh) = 0;
	virtual void deleteVertexBuffer(shared_ptr<VertexBufferHandle> vertexBufferHandle) = 0;

	// Textures
	virtual shared_ptr<TextureHandle> createTexture() = 0;
	virtual shared_ptr<TextureHandle> createTexture(shared_ptr<Image> image) = 0;
	virtual shared_ptr<TextureHandle> createTexture(shared_ptr<TextureBuildOptions> pTexOptions) = 0;
	virtual void deleteTexture(shared_ptr<TextureHandle> textureHandle) = 0;

	// Rendering
	virtual void stageTriangleRender(shared_ptr<RenderData> pRenderData) = 0;
	virtual void executeRender(RenderOptions &renderOptions) = 0;

	// Window
	virtual void setViewport(int nX, int nY, int nWidth, int nHeight) = 0;
	virtual shared_ptr<Viewport> getViewport() = 0;
	virtual void clearColor() = 0;
	virtual void clearDepth() = 0;
	virtual void clearDepthAndColor() = 0;

	// Downloaders
	virtual shared_ptr<Image> getRenderImage(RenderOptions::RenderTarget target) = 0;
	virtual shared_ptr<Image> getRenderImage(int x, int y, int width, int height, RenderOptions::RenderTarget target) = 0;
	virtual boost::shared_array<unsigned short> getRenderImagePixel(int x, int y, RenderOptions::RenderTarget target) = 0;

	virtual int getFrameBufferWidth() = 0;
	virtual int getFrameBufferHeight() = 0;
};