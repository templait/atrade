#include <QApplication>

#include "mainwindow.h"
#include <datasources/datasourcefilefactory.h>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("zoldnet");
	app.setApplicationName("atrade.terminal");

	DataSourceFactory::instance().registerUnit(new DataSourceFileFactory());

	MainWindow w;
	w.show();
	return app.exec();
}
