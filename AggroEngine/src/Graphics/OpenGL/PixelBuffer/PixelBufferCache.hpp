#pragma once

#include "Graphics.hpp"
#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
using namespace std;

/**
* Manages pixel buffer object. When a new size is requested, a new PBO is generated.
*
* author: wcrane
* since: 2016-08-28
**/
class PixelBufferCache
{
private:
	boost::unordered_map<string, GLuint> m_sizeToPBO;

public:
	PixelBufferCache();

	GLuint getPixelBuffer(int size, string ns = "");
};