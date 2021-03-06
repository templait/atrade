#include <QApplication>

#include "mainwindow.h"
#include <datasources/filedatasource/filedatasourcefactory.h>
#include <datasources/quikdatasource/quikdatasourcefactory.h>
#include <indicators/lualineindicator/lualineindicatorfactory.h>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("zoldnet");
	app.setApplicationName("atrade.terminal");

	DataSourceFactory::instance().registerUnit(new FileDataSourceFactory());
	DataSourceFactory::instance().registerUnit(new QuikDataSourceFactory());

	IndicatorFactory::instance().registerUnit(new LuaLineIndicatorFactory(LuaLineIndicatorFactory::IndicatorMA));

	MainWindow w;
	w.show();
	return app.exec();
}

// Параметры конфига

// DataSourceFileDir					- каталог с дампами катироваок
// LuaDir								- каталог со скриптами
// DataSourceQUIK=192.168.9.156:5000	- адрес и порт сервера данных QUIK
