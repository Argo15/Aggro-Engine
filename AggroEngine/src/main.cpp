#include <QtWidgets/QApplication>
#include <memory>
#include "Widgets/MainWindow.hpp"

int main(int argc, char* argv[]) {
	std::unique_ptr<QCoreApplication> app(new QApplication(argc, argv));
	MainWindow mainWindow;
	mainWindow.setGeometry(80, 150, 1600, 900);
	mainWindow.setWindowTitle("Aggro Engine");
	mainWindow.show();
	return app->exec();
}