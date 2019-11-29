#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <memory>
#include "Image.hpp"
#include "TextureHandle.hpp"
#include "TextureBuildOptions.hpp"
#include "Mesh.hpp"
#include "VertexBufferHandle.hpp"
#include "RenderData.hpp"
#include "RenderOptions.hpp"
#include "Viewport.hpp"
#include "GraphicsInitOptions.hpp"
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

	virtual void init(shared_ptr<GraphicsInitOptions> options) = 0;

	// Vertex buffers
	// Returns a vertex buffer handle
	virtual shared_ptr<VertexBufferHandle> createVertexBuffer(shared_ptr<Mesh> mesh) = 0;
	virtual void deleteVertexBuffer(shared_ptr<VertexBufferHandle> vertexBufferHandle) = 0;

	// Textures
	virtual shared_ptr<TextureHandle> createTexture() = 0;
	virtual shared_ptr<TextureHandle> createTexture(shared_ptr<ImageUC> image) = 0;
	virtual shared_ptr<TextureHandle> createTexture(shared_ptr<TextureBuildOptions> pTexOptions) = 0;
	virtual void deleteTexture(shared_ptr<TextureHandle> textureHandle) = 0;

	// Rendering
	virtual void stageRender(shared_ptr<RenderData> pRenderData) = 0;
	virtual void executeRender(RenderOptions &renderOptions) = 0;
	virtual void drawScreen(RenderOptions &renderOptions) = 0;

	// Window
	virtual void setViewport(int nX, int nY, int nWidth, int nHeight) = 0;
	virtual shared_ptr<Viewport> getViewport() = 0;
	virtual void clearColor(int frameBufferId) = 0;
	virtual void clearDepth(int frameBufferId) = 0;
	virtual void clearDepthAndColor(int frameBufferId) = 0;

	// Downloaders
	virtual shared_ptr<ImageUC> getSelectionImage(int x, int y, int width, int height) = 0;
	virtual shared_ptr<ImageF> getDepthImage(int x, int y, int width, int height) = 0;
	virtual shared_ptr<unsigned char> getSelectionImagePixel(int x, int y) = 0;
	virtual shared_ptr<float> getDepthImagePixel(int x, int y) = 0;

	virtual int getFrameBufferWidth() = 0;
	virtual int getFrameBufferHeight() = 0;
};