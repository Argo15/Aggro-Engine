#pragma once

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <unordered_set>
#include "GLResourceWidget.hpp"

enum FileType
{
	DIRECTORY,
	IMAGE,
	MESH,
	OTHER
};

class ResourceItem : public QWidget
{
private:
	GLResourceWidget *m_parent;
	QModelIndex m_idx;
	QString m_filepath;
	FileType m_fileType;
	unordered_set<string> m_supportedImages;
	unordered_set<string> m_supportedMeshes;

protected:
	void mousePressEvent(QMouseEvent *event);

public:
	ResourceItem(GLResourceWidget *parent, QModelIndex idx);

	FileType getFileType();
};