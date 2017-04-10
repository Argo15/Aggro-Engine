#include "EngineContext.hpp"

EngineContext::EngineContext()
	: m_jobManager(new JobManager())
	, m_scene(new Scene())
	, m_renderOptions(new RenderOptions())
	, m_resources(new Resources())
{
}

shared_ptr<Scene> EngineContext::getScene()
{
	return m_scene;
}

shared_ptr<RenderOptions> EngineContext::getRenderOptions()
{
	return m_renderOptions;
}

shared_ptr<JobManager> EngineContext::getJobManager()
{
	return m_jobManager;
}

shared_ptr<Resources> EngineContext::getResources()
{
	return m_resources;
}