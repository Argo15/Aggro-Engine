#include "CameraUpdateJob.hpp"
#include "Config.hpp"

CameraUpdateJob::CameraUpdateJob(shared_ptr<EngineContext> engineContext,
	shared_ptr<CameraController> cameraController,
	shared_ptr<KeyboardState> keyboard,
	shared_ptr<MouseState> mouse)
	: TickingJob(gConfig->getProperties().getIntProperty("engine.camera_fps"))
	, m_engineContext(engineContext)
	, m_cameraController(cameraController)
	, m_keyboard(keyboard)
	, m_mouse(mouse)
{
}

void CameraUpdateJob::tick(int fps)
{
	m_cameraController->handleMouseInput(m_engineContext->getScene()->getCamera(), *m_mouse.get(), 0.004f);
	m_cameraController->handleKeyboardInput(m_engineContext->getScene()->getCamera(), *m_keyboard.get(), 1.f / fps);
	m_mouse->clear();
}