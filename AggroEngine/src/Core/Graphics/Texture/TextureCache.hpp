#pragma once

#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
#include "LoadableTextureHandle.hpp"
#include "FreeImageImporter.hpp"
#include "Graphics.hpp"
#include "JobManager.hpp"
#include "Resources.hpp"
using namespace std;

class TextureCache
{
private:
	shared_ptr<ImageImporter> m_imageImporter;
	shared_ptr<Graphics> m_graphics;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;

	boost::unordered_map<int, shared_ptr<TextureHandle>> m_pathsToTexture;
	boost::unordered_map<int, shared_ptr<LoadableTextureHandle>> m_loadingTextures;

public:
	TextureCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources);

	shared_ptr<TextureHandle> getTexture(int sFilename);

	void finishLoading(int sFilename, shared_ptr<TextureHandle> handle);
};