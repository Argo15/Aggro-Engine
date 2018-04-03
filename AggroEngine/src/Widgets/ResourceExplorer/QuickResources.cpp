#include "QuickResources.hpp"
#include "Config.hpp"

QuickResources::QuickResources(GLResourceWidget *resourceWidget)
	: QListWidget()
{
	vector<string> quickPaths = Config::instance().getProperties().getStringArrayProperty("engine.quick_resources");
	for (auto path : quickPaths)
	{
		addItem(QString::fromStdString(path));
	}

	connect(this, &QuickResources::itemClicked, [resourceWidget](auto item) {
		QString dir = QDir::current().path() + "/Resources/" + item->text();
		resourceWidget->getDirModel().setRootPath(dir);
	});
}