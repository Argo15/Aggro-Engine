#pragma once

#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
#include "GraphicsInitOptions.hpp"
#include "GBuffer.hpp"
#include "AsyncTextureHandle.hpp"
using namespace std;

/**
* Manages pixel buffer objects.
*
* author: wcrane
* since: 2016-08-28
**/
class PixelBufferCache
{
private:
	GLuint *m_selectionPBOs;
	GLsizei m_width;
	GLsizei m_height;
	int m_curIndex;
	int m_numBuffs;

	// Textures uploads
	boost::unordered_map<GLuint, shared_ptr<TextureBuildOptions>> m_texOptions;
	boost::unordered_map<GLuint, shared_ptr<AsyncTextureHandle>> m_texHandles;
	boost::unordered_map<GLuint, GLuint> m_texToPBO;

public:
	PixelBufferCache(shared_ptr<GraphicsInitOptions> options);

	void writeSelectionBuffer(shared_ptr<GBuffer> gBuffer);
	shared_ptr<ImageUC> getSelectionImage(int x, int y, int width, int height);

	shared_ptr<TextureHandle> uploadTexturePixels(shared_ptr<TextureBuildOptions> texOptions);
	void resolveTextures(shared_ptr<BufferSyncContext> syncContext);
};