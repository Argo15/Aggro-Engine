#include "TextureCache.hpp"

TextureCache::TextureCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources)
	: m_graphics(graphics)
	, m_jobManager(jobManager)
	, m_imageImporter(new FreeImageImporter())
	, m_pathsToTexture()
	, m_resources(resources)
{

}

shared_ptr<TextureHandle> TextureCache::getTexture(int imageId)
{
	boost::optional<string> imagePathOpt = m_resources->getPathForId(imageId);
	if (!imagePathOpt)
	{
		cout << "unknown image id " << imageId << endl;
		return shared_ptr<TextureHandle>();
	}
	const string imagePath = imagePathOpt.get();

	// Load texture if not loaded
	if (m_pathsToTexture.find(imageId) == m_pathsToTexture.end())
	{
		m_jobManager->add(shared_ptr<Job>(new Job([this, imageId, imagePath]()
		{
			shared_ptr<Image> image(m_imageImporter->importImage(imagePath));
			m_jobManager->addGraphicsJob(shared_ptr<Job>(new Job([this, image, imageId]()
			{
				shared_ptr<TextureBuildOptions> texOptions(new TextureBuildOptions(image));
				shared_ptr<TextureHandle> newTex(m_graphics->createTexture(texOptions));
				finishLoading(imageId, newTex);
			})));
		})));
		shared_ptr<LoadableTextureHandle> loadingTexture(new LoadableTextureHandle());
		m_loadingTextures[imageId] = loadingTexture;
		m_pathsToTexture[imageId] = loadingTexture;
	}

	return m_pathsToTexture[imageId];
}

void TextureCache::finishLoading(int imageId, shared_ptr<TextureHandle> handle)
{
	if (m_loadingTextures.find(imageId) != m_loadingTextures.end())
	{
		m_loadingTextures[imageId]->setHandle(handle);
		m_loadingTextures.erase(imageId);
	}
}