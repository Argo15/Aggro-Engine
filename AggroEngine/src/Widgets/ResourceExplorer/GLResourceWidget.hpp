#pragma once

#include "FlowLayout.hpp"
#include "FreeImageImporter.hpp"
#include <QWidget>
#include <QFileSystemModel>
#include <memory>
using namespace std;

class GLResourceWidget : public QWidget
{
private:
	QFileSystemModel m_dirModel;
	QModelIndex m_rootIdx;
	FlowLayout m_layout;
	shared_ptr<FreeImageImporter> m_imgImporter;

	void _setPath(QString path);

public:
	GLResourceWidget(QWidget *parent = nullptr);

	QFileSystemModel &getDirModel();
	shared_ptr<FreeImageImporter> imgImporter();
};