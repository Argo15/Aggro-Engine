#include "GLResourceWidget.hpp"
#include <QPushButton>
#include <QFileSystemModel>
#include "ResourceItem.hpp"
#include "GLMeshWidget.hpp"
#include "FileTypes.hpp"

GLResourceWidget::GLResourceWidget(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
	, m_dirModel()
	, m_rootIdx()
	, m_layout(10, 0, 0)
	, m_imgImporter(new FreeImageImporter())
	, m_context(context)
	, m_currentWidgets()
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
	while (!m_currentWidgets.empty())
	{
		m_currentWidgets.back()->hide();
		m_currentWidgets.pop_back();
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

		QString path = m_dirModel.filePath(modelIdx);
		string sPath = path.toStdString();
		if (m_pathToWidget.find(sPath) != m_pathToWidget.end())
		{
			m_pathToWidget[sPath]->show();
			m_currentWidgets.push_back(m_pathToWidget[sPath]);
			continue;
		}

		FileType fileType = gFileTypes->fromPath(path);
		switch (fileType)
		{
			case DIRECTORY: 
			case IMAGE: 
				_addWidget(sPath, new ResourceItem(this, modelIdx));
				break;
			case MESH:
				_addWidget(sPath, new ResourceItem(this, modelIdx, new GLMeshWidget(path, m_context, this)));
				break;
			default: break;
		}
	}
}

void GLResourceWidget::_addWidget(string &path, QWidget *widget)
{
	shared_ptr<QWidget> newItem = shared_ptr<QWidget>(widget);
	m_layout.addWidget(newItem.get());
	m_currentWidgets.push_back(newItem);
	m_pathToWidget[path] = newItem;
}

shared_ptr<FreeImageImporter> GLResourceWidget::imgImporter()
{
	return m_imgImporter;
}