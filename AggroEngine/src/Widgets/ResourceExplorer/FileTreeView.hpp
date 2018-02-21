#pragma once

#include <QTreeView>
#include <QFileSystemModel>
#include <memory>
#include <unordered_set>
using namespace std;

class FileTreeView : public QTreeView
{
	Q_OBJECT

private:
	shared_ptr<QFileSystemModel> m_dirModel;
	unordered_set<string> m_supportedImages;
	unordered_set<string> m_supportedMeshes;

protected:
	void mousePressEvent(QMouseEvent *event);

public:
	FileTreeView(QWidget *parent = nullptr);
};