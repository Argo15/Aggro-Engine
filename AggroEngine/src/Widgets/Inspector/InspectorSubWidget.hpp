#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "SceneNode.hpp"

class InspectorSubWidget : public QWidget
{
protected:
	shared_ptr<QVBoxLayout> m_layout;

	virtual void _refresh(shared_ptr<SceneNode> node) = 0;

public:
	InspectorSubWidget(QWidget *parent = nullptr);

	void refresh(shared_ptr<SceneNode> node);
};