#pragma once

#include "FlowLayout.hpp"
#include "FreeImageImporter.hpp"
#include "EngineContext.hpp"
#include "GLWidget.hpp"
#include <QWidget>
#include <QFileSystemModel>
#include <memory>
#include <unordered_map>
using namespace std;

class GLResourceWidget : public QWidget
{
private:
	QFileSystemModel m_dirModel;
	QModelIndex m_rootIdx;
	FlowLayout m_layout;
	shared_ptr<FreeImageImporter> m_imgImporter;
	shared_ptr<EngineContext> m_context;
	vector<shared_ptr<QWidget>> m_currentWidgets;
	unordered_map<string, shared_ptr<QWidget>> m_pathToWidget;

	void _setPath(QString path);
	void _addWidget(string &path, QWidget *widget);

public:
	GLResourceWidget(shared_ptr<EngineContext> context, QWidget *parent = nullptr);

	QFileSystemModel &getDirModel();
	shared_ptr<FreeImageImporter> imgImporter();
};