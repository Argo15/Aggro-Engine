#include "CameraUpdateJob.hpp"
#include "Config.hpp"

CameraUpdateJob::CameraUpdateJob(shared_ptr<EngineContext> engineContext,
	shared_ptr<CameraController> cameraController,
	shared_ptr<KeyboardState> keyboard,
	shared_ptr<MouseState> mouse)
	: m_engineContext(engineContext)
	, m_cameraController(cameraController)
	, m_keyboard(keyboard)
	, m_mouse(mouse)
	, m_cameraTime(0)
{
	const Properties& props = gConfig->getProperties();
	m_cameraUpdateFps = props.getIntProperty("engine.camera_fps");
	m_timer.Init();

	m_function = [this]() {
		while (true)
		{
			if (m_engineContext && m_engineContext->getScene())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
				unsigned int millis = m_timer.GetMS();
				m_cameraTime += millis;
				if (m_cameraTime > 1000 / m_cameraUpdateFps)
				{
					int fps = 1000 / m_cameraTime;
					m_cameraController->handleMouseInput(m_engineContext->getScene()->getCamera(), *m_mouse.get(), 1.f / fps);
					m_cameraController->handleKeyboardInput(m_engineContext->getScene()->getCamera(), *m_keyboard.get(), 1.f / fps);
					m_mouse->clear();
					m_cameraTime = 0;
				}
			}
		}
		return;
	};
}