#pragma once

#include "TextureHandle.hpp"
#include "FreeImageImporter.hpp"
#include "Graphics.hpp"
#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
using namespace std;

class TextureCache
{
private:
	shared_ptr<ImageImporter> m_imageImporter;
	shared_ptr<Graphics> m_graphics;

	boost::unordered_map<string, shared_ptr<TextureHandle>> m_pathsToTexture;

public:
	TextureCache(shared_ptr<Graphics> graphics);

	shared_ptr<TextureHandle> getTexture(string sFilename);
};