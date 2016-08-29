#pragma once

#include <QWidget>
#include <QTreeWidget>

class SceneTreeWidget : public QTreeWidget
{
protected:
	void mousePressEvent(QMouseEvent *event);

public: 
	SceneTreeWidget(QWidget *parent = nullptr);
};