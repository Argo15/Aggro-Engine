#pragma once

#include "SceneNodeImporter.hpp"
#include "CachedSceneNodeHandler.hpp"
#include "JobManager.hpp"
#include "Resources.hpp"
#include "Locks.hpp"
#include <memory>
using namespace std;

class SceneNodeImporter;
class CachedSceneNodeHandler;

class SceneNodeCache : public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<SceneNodeImporter> m_sceneNodeImporter;
	shared_ptr<JobManager> m_jobManager;
	shared_ptr<Resources> m_resources;

	boost::unordered_map<int, shared_ptr<CachedSceneNodeHandler>> m_idToSceneNode;

public:
	SceneNodeCache(shared_ptr<JobManager> jobManager, shared_ptr<Resources> resources);

	shared_ptr<CachedSceneNodeHandler> getSceneNode(int sceneNodeResourceId);
	shared_ptr<SceneNode> waitForSceneNode(int sceneNodeResourceId);
};