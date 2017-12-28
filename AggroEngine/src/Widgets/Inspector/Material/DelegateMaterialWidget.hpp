#pragma once

#include <QLabel>
#include <QPushButton>
#include "InspectorWidget.hpp"
#include "Locks.hpp"

class DelegateMaterialWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QPushButton> m_editBaseBtn;
	shared_ptr<QPushButton> m_copyBaseBtn;
	boost::mutex m_lock;

protected:
	virtual void _refresh(SceneNode *newNode);

public:
	DelegateMaterialWidget(InspectorWidget *parent, shared_ptr<Scene> scene);
};