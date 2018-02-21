#include "FileTreeView.hpp"
#include <QFileSystemModel>
#include <QHeaderView>
#include <QFileIconProvider>
#include <QMouseEvent>
#include <QTreeWidgetItem>
#include <QDrag>
#include <QMimeData>
#include "config.hpp"

FileTreeView::FileTreeView(QWidget *parent)
	: QTreeView(parent)
	, m_dirModel(new QFileSystemModel())
{
	m_dirModel->setFilter(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

	QString dir = QDir::current().path() + "/Resources";
	m_dirModel->setRootPath(dir);
	setModel(m_dirModel.get());
	setRootIndex(m_dirModel->index(dir));

	hideColumn(1);
	hideColumn(2);
	hideColumn(3);
	header()->resizeSection(0, 400);
	setDragEnabled(true);
	vector<string> imgExtensions = gConfig->getProperties().getStringArrayProperty("engine.tex_extensions");
	std::copy(imgExtensions.begin(), imgExtensions.end(), 
			  std::inserter(m_supportedImages, m_supportedImages.end()));
	vector<string> meshExtensions = gConfig->getProperties().getStringArrayProperty("engine.mesh_extensions");
	std::copy(meshExtensions.begin(), meshExtensions.end(),
		std::inserter(m_supportedMeshes, m_supportedMeshes.end()));
}

void FileTreeView::mousePressEvent(QMouseEvent *event)
{
	QTreeView::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		QString filePath = m_dirModel->filePath(indexAt(event->pos()));
		if (filePath.contains("."))
		{
			QString extension = filePath.split(".", QString::SkipEmptyParts).last();
			string mimeType = "";
			if (m_supportedImages.find(extension.toStdString()) != m_supportedImages.end())
			{
				mimeType = "application/x-texture";
			}
			else if (m_supportedMeshes.find(extension.toStdString()) != m_supportedMeshes.end())
			{
				mimeType = "application/x-mesh";
			}
			else
			{
				return;
			}

			QDir workingDirectory = QDir::current();
			filePath = workingDirectory.relativeFilePath(filePath);

			QDrag *drag = new QDrag(this);
			QMimeData *mimeData = new QMimeData();
			mimeData->setData(QString::fromStdString(mimeType), filePath.toLocal8Bit());
			drag->setMimeData(mimeData);
			Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
		}
	}
}