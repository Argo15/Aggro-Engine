#pragma once

#include "EngineContext.hpp"
#include "Job.hpp"
#include "FreeRoamCameraController.hpp"
#include "timer.hpp"

class EngineContext;

class CameraUpdateJob : public Job
{
private:
	shared_ptr<EngineContext> m_engineContext;
	shared_ptr<CameraController> m_cameraController;
	shared_ptr<KeyboardState> m_keyboard;
	shared_ptr<MouseState> m_mouse;
	Timer m_timer;
	int m_cameraUpdateFps;
	long m_cameraTime;

public:
	CameraUpdateJob(shared_ptr<EngineContext> engineContext,
		shared_ptr<CameraController> cameraController,
		shared_ptr<KeyboardState> keyboard,
		shared_ptr<MouseState> mouse);
};