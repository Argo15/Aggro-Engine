#include "SceneTreeWidget.hpp"

SceneTreeWidget::SceneTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	setMouseTracking(true);
}

void SceneTreeWidget::mousePressEvent(QMouseEvent *event)
{
	selectionModel()->clearSelection();
	QTreeWidget::mousePressEvent(event);
}