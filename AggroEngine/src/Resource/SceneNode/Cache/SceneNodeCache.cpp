#include "SceneNodeCache.hpp"
#include "AssimpSceneNodeImporter.hpp"
#include "SceneNode.hpp"

SceneNodeCache::SceneNodeCache(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources)
	: m_sceneNodeImporter(new AssimpSceneNodeImporter(jobManager))
	, m_jobManager(jobManager)
	, m_resources(resources)
	, m_idToSceneNode()
{

}

shared_ptr<CachedSceneNodeHandler> SceneNodeCache::getSceneNode(int sceneNodeResourceId)
{
	boost::lock_guard<SceneNodeCache> guard(*this);
	boost::optional<std::string> filepath = m_resources->getPathForId(sceneNodeResourceId);
	if (!filepath)
	{
		return shared_ptr<CachedSceneNodeHandler>();
	}
	if (m_idToSceneNode.find(sceneNodeResourceId) == m_idToSceneNode.end())
	{
		m_idToSceneNode[sceneNodeResourceId] = shared_ptr<CachedSceneNodeHandler>(new CachedSceneNodeHandler());
		m_jobManager->add(shared_ptr<Job>(new Job([this, sceneNodeResourceId, filepath]()
		{
			shared_ptr<SceneNode> newSceneNode = m_sceneNodeImporter->importSceneNode(filepath.get());
			m_idToSceneNode[sceneNodeResourceId]->updateSceneNode(newSceneNode);
			m_idToSceneNode[sceneNodeResourceId]->notifyReady();
		})));
	}
	return m_idToSceneNode[sceneNodeResourceId];
}

shared_ptr<SceneNode> SceneNodeCache::waitForSceneNode(int sceneNodeResourceId)
{
	boost::lock_guard<SceneNodeCache> guard(*this);
	boost::optional<std::string> filepath = m_resources->getPathForId(sceneNodeResourceId);
	if (!filepath)
	{
		return shared_ptr<SceneNode>();
	}
	if (m_idToSceneNode.find(sceneNodeResourceId) == m_idToSceneNode.end())
	{
		m_idToSceneNode[sceneNodeResourceId] = shared_ptr<CachedSceneNodeHandler>(new CachedSceneNodeHandler());
		shared_ptr<SceneNode> newSceneNode = m_sceneNodeImporter->importSceneNode(filepath.get());
		m_idToSceneNode[sceneNodeResourceId]->updateSceneNode(newSceneNode);
		m_idToSceneNode[sceneNodeResourceId]->notifyReady();
	}
	return m_idToSceneNode[sceneNodeResourceId]->get();
}