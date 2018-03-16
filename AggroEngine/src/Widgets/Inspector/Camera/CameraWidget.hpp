#pragma once

#include <QLabel>
#include <QPushButton>
#include "InspectorWidget.hpp"
#include "Locks.hpp"

class CameraWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QPushButton> m_setCurrent;
	shared_ptr<QPushButton> m_leaveCamera;
	boost::mutex m_lock;

	void _refresh(CameraComponent *camera);

protected:
	virtual void _refresh(SceneNode *newNode);

public:
	CameraWidget(InspectorWidget *parent, shared_ptr<EngineContext> context);
};