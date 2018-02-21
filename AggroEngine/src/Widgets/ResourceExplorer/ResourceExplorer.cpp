#include "ResourceExplorer.hpp"
#include <QHBoxLayout>
#include "FileTreeView.hpp"

ResourceExplorer::ResourceExplorer(shared_ptr<EngineContext> context, QWidget *parent)
	: QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout;

	QTreeView *treeWidget = new FileTreeView(this);
	layout->addWidget(treeWidget);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
}