#include "TextureCache.hpp"

TextureCache::TextureCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager)
	: m_graphics(graphics)
	, m_jobManager(jobManager)
	, m_imageImporter(shared_ptr<ImageImporter>(new FreeImageImporter()))
	, m_pathsToTexture()
	, m_defaultTexture()
{

}

shared_ptr<TextureHandle> TextureCache::getTexture(string sFilename)
{
	if (!m_defaultTexture)
	{
		m_defaultTexture = m_graphics->createTexture();
	}

	// Load texture if not loaded
	if (m_pathsToTexture.find(sFilename) == m_pathsToTexture.end())
	{
		m_jobManager->add(shared_ptr<Job>(new Job([this, sFilename]() 
		{
			shared_ptr<Image> image(m_imageImporter->importImage(sFilename));
			m_jobManager->addGraphicsJob(shared_ptr<Job>(new Job([this, image, sFilename]()
			{
				shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(image));
				shared_ptr<TextureHandle> newTex(m_graphics->createTexture(texOptions));
				finishLoading(sFilename, newTex);
			})));
		})));
		shared_ptr<LoadableTextureHandle> loadingTexture(new LoadableTextureHandle(m_defaultTexture));
		m_loadingTextures[sFilename] = loadingTexture;
		m_pathsToTexture[sFilename] = loadingTexture;
	}

	return m_pathsToTexture[sFilename];
}

void TextureCache::finishLoading(string sFilename, shared_ptr<TextureHandle> handle)
{
	if (m_loadingTextures.find(sFilename) != m_loadingTextures.end())
	{
		m_loadingTextures[sFilename]->setHandle(handle);
		m_loadingTextures.erase(sFilename);
	}
}