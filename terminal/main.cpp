#include <QApplication>

#include "mainwindow.h"
//#include "datasourcequik.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("zoldnet");
	app.setApplicationName("atrade.terminal");
	MainWindow w;

	w.show();
	return app.exec();
}
