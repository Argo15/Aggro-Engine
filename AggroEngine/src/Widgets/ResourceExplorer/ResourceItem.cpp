#include "ResourceItem.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileIconProvider>
#include <QPainter>
#include <QDrag>
#include <QMimeData>
#include "Config.hpp"

ResourceItem::ResourceItem(GLResourceWidget *parent, QModelIndex idx, QWidget *iconWidget)
	: QWidget(parent)
	, m_parent(parent)
	, m_idx(idx)
	, m_fileType(OTHER)
{
	QBoxLayout *layout = new QVBoxLayout;
	QFileSystemModel &dirModel = parent->getDirModel();
	QFileIconProvider *iconProvider = dirModel.iconProvider();
	QString filename = dirModel.fileName(idx);
	m_filepath = dirModel.filePath(idx);
	m_fileType = gFileTypes->fromPath(m_filepath);

	if (iconWidget == nullptr)
	{
		QLabel *iconLbl = new QLabel("");
		QSize iconSize = QSize(48, 48);
		QPixmap pixmap;
		if (dirModel.isDir(idx))
		{
			pixmap = iconProvider->icon(QFileIconProvider::IconType::Folder).pixmap(iconSize);
		}
		else
		{
			bool loaded = false;
			if (m_fileType == IMAGE)
			{
				shared_ptr<ImageUC> image = parent->imgImporter()->importImage(m_filepath.toStdString());
				QImage qimage(image->getData().get(), image->getWidth(), image->getHeight(), QImage::Format_RGBA8888);
				if (!qimage.isNull())
				{
					qimage = qimage.scaled(QSize(100, 100));
					pixmap = QPixmap::fromImage(qimage);
					loaded = true;
				}
			}
			if (!loaded)
			{
				pixmap = iconProvider->icon(QFileIconProvider::IconType::File).pixmap(iconSize);
			}
		}
		iconLbl->setPixmap(pixmap);
		iconLbl->setContentsMargins(0, 0, 0, 0);
		iconLbl->setAlignment(Qt::AlignCenter);
		iconWidget = iconLbl;
	}
	layout->addWidget(iconWidget);

	QLabel *name = new QLabel(filename);
	name->setAlignment(Qt::AlignCenter);
	name->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(name);

	setLayout(layout);
	setMouseTracking(true);
}

void ResourceItem::mousePressEvent(QMouseEvent *event)
{
	QFileSystemModel &dirModel = m_parent->getDirModel();
	if (dirModel.isDir(m_idx))
	{
		dirModel.setRootPath(m_filepath);
	}
	else if (m_fileType == IMAGE || m_fileType == MESH)
	{
		string mimeType = m_fileType == IMAGE ? "application/x-texture" : "application/x-mesh";

		QDir workingDirectory = QDir::current();
		m_filepath = workingDirectory.relativeFilePath(m_filepath);

		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData();
		mimeData->setData(QString::fromStdString(mimeType), m_filepath.toLocal8Bit());
		drag->setMimeData(mimeData);
		Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
	}
}

FileType ResourceItem::getFileType()
{
	return m_fileType;
}