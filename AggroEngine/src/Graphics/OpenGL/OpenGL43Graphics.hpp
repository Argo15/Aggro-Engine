#pragma once

class OpenGL43Graphics;

#include <queue>
#include <memory>
#include "ShaderStore.hpp"
#include "GBuffer.hpp"
#include "Graphics.hpp"


/**
 * OpenGL graphics that only guarentees support for OpenGL 4.3 or above.
 *
 * author: wcrane
 * since: 2013-11-03
**/
class OpenGL43Graphics : public Graphics
{
private:
	std::queue<shared_ptr<RenderData>> renderQueue;
	ShaderStore m_shaderStore;
	shared_ptr<GBuffer> m_gBuffer;

	void _drawScreen(RenderOptions &renderOptions, float nX1, float nY1, float nX2, float nY2);

public:
	OpenGL43Graphics();
	~OpenGL43Graphics();

	void init();

	shared_ptr<VertexBufferHandle> createVertexBuffer(shared_ptr<Mesh> mesh); 
	void deleteVertexBuffer(shared_ptr<VertexBufferHandle> nHandle);
	
	shared_ptr<TextureHandle> createTexture();
	shared_ptr<TextureHandle> createTexture(shared_ptr<TextureBuildOptions> pTexOptions);
	void deleteTexture(shared_ptr<TextureHandle> textureHandle);

	void stageTriangleRender(shared_ptr<RenderData> pRenderData);
	void executeRender(RenderOptions &renderOptions);

	void setViewport(int nX, int nY, int nWidth, int nHeight);
	void clearColor();
	void clearDepth();
	void clearDepthAndColor();

	ShaderStore getShaderStore();
};