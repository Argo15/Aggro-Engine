#pragma once

#include <QWidget>
#include <QTreeWidget>
#include "SceneGraphTree.hpp"

class SceneTreeWidget : public QTreeWidget
{
	Q_OBJECT

private:
	SceneGraphTree *m_tree;

protected:
	void mousePressEvent(QMouseEvent *event);

public: 
	SceneTreeWidget(SceneGraphTree *parent = nullptr);
};