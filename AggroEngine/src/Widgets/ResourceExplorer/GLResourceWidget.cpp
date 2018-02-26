#include "GLResourceWidget.hpp"
#include <QPushButton>
#include <QFileSystemModel>
#include "ResourceItem.hpp"

GLResourceWidget::GLResourceWidget(QWidget *parent)
	: QWidget(parent)
	, m_dirModel()
	, m_rootIdx()
	, m_layout(10, 0, 0)
	, m_imgImporter(new FreeImageImporter())
{
	connect(&m_dirModel, &QFileSystemModel::directoryLoaded, this, &GLResourceWidget::_setPath);

	m_dirModel.setFilter(QDir::NoDot | QDir::Dirs | QDir::Files);
	QString dir = QDir::current().path() + "/Resources";
	m_rootIdx = m_dirModel.setRootPath(dir);

	setLayout(&m_layout);
}

QFileSystemModel &GLResourceWidget::getDirModel()
{
	return m_dirModel;
}

void GLResourceWidget::_setPath(QString path)
{
	QLayoutItem *wItem;
	while ((wItem = m_layout.takeAt(0)) != 0)
	{
		delete wItem->widget();
		delete wItem;
	}
	QModelIndex &dirIdx = m_dirModel.index(path);
	int rowCount = m_dirModel.rowCount(dirIdx);
	for (int row = 0; row < rowCount; row++)
	{
		QModelIndex &modelIdx = m_dirModel.index(row, 0, dirIdx);
		QString filename = m_dirModel.fileName(modelIdx);
		if (m_rootIdx == dirIdx && filename == "..")
		{
			continue;
		}
		ResourceItem *item = new ResourceItem(this, modelIdx);
		if (item->getFileType() != OTHER)
		{
			m_layout.addWidget(item);
		}
		else
		{
			delete item;
		}
	}
}

shared_ptr<FreeImageImporter> GLResourceWidget::imgImporter()
{
	return m_imgImporter;
}