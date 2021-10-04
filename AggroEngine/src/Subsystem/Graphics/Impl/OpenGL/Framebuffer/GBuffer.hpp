#pragma once

class GBuffer;
class FrameBufferObject;

#include "FrameBufferObject.hpp"
#include "OpenGL43Graphics.hpp"
#include "Sync/BufferSyncContext.hpp"
#include "RenderChain.hpp"
#include "CommandTree/CommandTree.hpp"

class GBuffer : public FrameBufferObject
{
private:
	shared_ptr<TextureHandle> m_depthTex;
	shared_ptr<TextureHandle> m_normalTex;
	shared_ptr<TextureHandle> m_albedoTex;
	shared_ptr<TextureHandle> m_selectionTex;
	shared_ptr<TextureHandle> m_glowTex;
	shared_ptr<TextureHandle> m_whiteTexture;
	shared_ptr<VertexBufferHandle> m_screenVBO;
	OpenGL43Graphics *m_graphics;
	shared_ptr<BufferSyncContext> m_syncContext;

	shared_ptr<CommandTree> m_commands;

public:
	GBuffer(OpenGL43Graphics *graphics, int width, int height, shared_ptr<BufferSyncContext> syncContext);

	void drawToBuffer(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain);
	void drawToBufferOld(RenderOptions &renderOptions, shared_ptr<RenderChain> renderChain);

	void bindDepthTex();
	void bindNormalTex();
	void bindAlbedoTex();
	void bindSelectionTex();

	shared_ptr<TextureHandle> getDepthTex();
	shared_ptr<TextureHandle> getNormalTex();
	shared_ptr<TextureHandle> getAlbedoTex();
	shared_ptr<TextureHandle> getSelectionTex();
	shared_ptr<TextureHandle> getGlowTex();

	GLenum getSelectionColorAttachment();
};