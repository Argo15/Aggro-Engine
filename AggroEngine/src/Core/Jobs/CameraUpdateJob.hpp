#pragma once

#include "EngineContext.hpp"
#include "Job.hpp"
#include "FreeRoamCameraController.hpp"
#include "TickingJob.hpp"

class EngineContext;

class CameraUpdateJob : public TickingJob
{
private:
	shared_ptr<EngineContext> m_engineContext;
	shared_ptr<CameraController> m_cameraController;
	shared_ptr<KeyboardState> m_keyboard;
	shared_ptr<MouseState> m_mouse;

public:
	CameraUpdateJob(shared_ptr<EngineContext> engineContext,
		shared_ptr<CameraController> cameraController,
		shared_ptr<KeyboardState> keyboard,
		shared_ptr<MouseState> mouse);

	void tick(int fps);
};