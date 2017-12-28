#include "SceneTreeWidget.hpp"
#include <QDrag>
#include <QMimeData>
#include <iostream>
using namespace std;

SceneTreeWidget::SceneTreeWidget(SceneGraphTree *parent)
	: QTreeWidget(parent)
	, m_tree(parent)
{
	setMouseTracking(true);
	setDragEnabled(true);
}

void SceneTreeWidget::mousePressEvent(QMouseEvent *event)
{
	selectionModel()->clearSelection();
	QTreeWidget::mousePressEvent(event);

	if (event->button() == Qt::LeftButton)
	{
		QTreeWidgetItem *item = itemAt(event->pos());
		if (item != nullptr)
		{
			SceneNodeTreeItem *sceneNodeItem = static_cast<SceneNodeTreeItem *>(item);
			if (sceneNodeItem->getSceneNode() && sceneNodeItem->getSceneNode()->isBaseMaterialNode())
			{
				QDrag *drag = new QDrag(this);
				QByteArray data = QByteArray();
				QMimeData *mimeData = new QMimeData();
				mimeData->setData("application/x-materialdata", QByteArray());
				drag->setMimeData(mimeData);

				Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
			}
		}
	}
}