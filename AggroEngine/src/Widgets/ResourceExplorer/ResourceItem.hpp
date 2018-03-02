#pragma once

#include <QWidget>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <unordered_set>
#include "GLResourceWidget.hpp"
#include "FileTypes.hpp"

class ResourceItem : public QWidget
{
private:
	GLResourceWidget *m_parent;
	QModelIndex m_idx;
	QString m_filepath;
	FileType m_fileType;

protected:
	void mousePressEvent(QMouseEvent *event);

public:
	ResourceItem(GLResourceWidget *parent, QModelIndex idx, QWidget *iconWidget = nullptr);

	FileType getFileType();
};