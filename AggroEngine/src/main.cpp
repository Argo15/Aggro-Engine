#include <QtWidgets/QApplication>
#include <memory>
#include "MainWindow.hpp"
#include "Config.hpp"

int main(int argc, char* argv[]) {
	std::unique_ptr<QCoreApplication> app(new QApplication(argc, argv));

	const Properties& props = gConfig->getProperties();
	vector<int> nDimensions = props.getIntArrayProperty("window_options.dimensions");
	
	MainWindow mainWindow;
	mainWindow.setGeometry(10, 50, nDimensions[0], nDimensions[1]);
	mainWindow.setWindowTitle(QString::fromStdString(props.getStringProperty("window_options.title")));
	mainWindow.show();

	return app->exec();
}