#include "QuickResources.hpp"
#include "Config.hpp"

QuickResources::QuickResources(GLResourceWidget *resourceWidget)
	: QListWidget()
{
	vector<string> quickPaths = gConfig->getProperties().getStringArrayProperty("engine.quick_resources");
	for (auto path : quickPaths)
	{
		addItem(QString::fromStdString(path));
	}

	connect(this, &QuickResources::currentTextChanged, [resourceWidget](auto selected) {
		QString dir = QDir::current().path() + "/Resources/" + selected;
		resourceWidget->getDirModel().setRootPath(dir);
	});
}