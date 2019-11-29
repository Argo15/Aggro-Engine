#pragma once

#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
#include "GraphicsInitOptions.hpp"
#include "GBuffer.hpp"
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

public:
	PixelBufferCache(shared_ptr<GraphicsInitOptions> options);

	void writeSelectionBuffer(shared_ptr<GBuffer> gBuffer);
	shared_ptr<ImageUC> getSelectionImage(int x, int y, int width, int height);
};