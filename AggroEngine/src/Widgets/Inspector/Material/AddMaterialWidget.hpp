#pragma once

#include <QLabel>
#include <QPushButton>
#include "InspectorWidget.hpp"
#include "Locks.hpp"

class AddMaterialWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QPushButton> m_addMaterialBtn;
	boost::mutex m_lock;

protected:
	virtual void _refresh(SceneNode *newNode);

public:
	AddMaterialWidget(InspectorWidget *parent = nullptr);
};