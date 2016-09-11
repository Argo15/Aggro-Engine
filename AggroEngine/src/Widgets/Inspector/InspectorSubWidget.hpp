#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "SceneNode.hpp"

class InspectorSubWidget : public QWidget
{
protected:
	shared_ptr<QVBoxLayout> m_layout;
	shared_ptr<SceneNode> m_currentNode;
	shared_ptr<SceneNode> m_lastActiveNode;

	virtual void _refresh(SceneNode *newNode) = 0;

public:
	InspectorSubWidget(QWidget *parent = nullptr);

	void refresh(shared_ptr<SceneNode> newNode);
};