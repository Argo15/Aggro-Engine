#include "CameraUpdateJob.hpp"
#include "Config.hpp"

CameraUpdateJob::CameraUpdateJob(shared_ptr<Camera> camera,
	shared_ptr<CameraController> cameraController,
	shared_ptr<KeyboardState> keyboard,
	shared_ptr<MouseState> mouse)
	: TickingJob(gConfig->getProperties().getIntProperty("engine.camera_fps"))
	, m_camera(camera)
	, m_cameraController(cameraController)
	, m_keyboard(keyboard)
	, m_mouse(mouse)
{
}

void CameraUpdateJob::tick(int fps)
{
	m_cameraController->handleMouseInput(m_camera, *m_mouse.get(), 0.004f);
	m_cameraController->handleKeyboardInput(m_camera, *m_keyboard.get(), 1.f / fps);
	m_mouse->clear();
}