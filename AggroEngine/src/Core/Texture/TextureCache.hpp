#pragma once

#include <string>
#include <memory>
#include <boost/unordered_map.hpp>
#include "LoadableTextureHandle.hpp"
#include "FreeImageImporter.hpp"
#include "Graphics.hpp"
#include "JobManager.hpp"
using namespace std;

class TextureCache
{
private:
	shared_ptr<ImageImporter> m_imageImporter;
	shared_ptr<Graphics> m_graphics;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<TextureHandle> m_defaultTexture;

	boost::unordered_map<string, shared_ptr<TextureHandle>> m_pathsToTexture;
	boost::unordered_map<string, shared_ptr<LoadableTextureHandle>> m_loadingTextures;

public:
	TextureCache(shared_ptr<Graphics> graphics, shared_ptr<JobManager> jobManager);

	shared_ptr<TextureHandle> getTexture(string sFilename);

	void finishLoading(string sFilename, shared_ptr<TextureHandle> handle);
};