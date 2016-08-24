#include "TextureCache.hpp"

TextureCache::TextureCache(shared_ptr<Graphics> graphics)
	: m_graphics(graphics)
	, m_imageImporter(shared_ptr<ImageImporter>(new FreeImageImporter()))
	, m_pathsToTexture()
{

}

shared_ptr<TextureHandle> TextureCache::getTexture(string sFilename)
{
	if (m_pathsToTexture.find(sFilename) == m_pathsToTexture.end())
	{
		shared_ptr<Image> image(m_imageImporter->importImage(sFilename));
		shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(image));
		shared_ptr<TextureHandle> newTex(m_graphics->createTexture(texOptions));
		m_pathsToTexture[sFilename] = newTex;
	}
	return m_pathsToTexture[sFilename];
}