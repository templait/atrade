#include "datasourcefilefactory.h"
#include "datasourcefile.h"

#include <log.h>
#include <tools.h>

#include <QDir>	// killme
#include <QSettings>

#define PRODUCT_ID "ab38fe10-d502-11e8-b568-0800200c9a66"

DataSourceFileFactory::DataSourceFileFactory()
	: DataSourceFactory::Unit(QString("File datasoure"), QUuid(PRODUCT_ID))
{

}

BDataSource *DataSourceFileFactory::create(const Configuration &configuration) const
{
	BDataSource* rv=nullptr;

	if(configuration.value().toUuid() == QUuid("ab38fe10-d502-11e8-b568-0800200c9a66"))
	{
		QSettings appSettings;
		QString path = QString("%1/%2/%3/%4.txt")
		        .arg(appSettings.value("DataSourceFileDir", "../DataSourceFile").toString())
				.arg(configuration["class"].value().toString())
				.arg(configuration["code"].value().toString())
				.arg(intervalToString(static_cast<ETimeInterval>(configuration["interval"].value().toInt())));
		rv = new DataSourceFile(path);
	}
	else
	{
		Log::error(QString("%1.invalid ProductID: \"%2\"").arg(__CLASS_NAME__).arg(configuration.value().toUuid().toString()));
	}

	return rv;
}

Configuration DataSourceFileFactory::defaultConfiguration() const
{
	QString name = __CLASS_NAME__;
	Configuration rv
	{
		{"class",	"",				QObject::tr("Класс",			name.toLocal8Bit())},
		{"code",	"",				QObject::tr("Код символла",		name.toLocal8Bit())},
		{"interval",IntervalM15,	QObject::tr("Интервал",			name.toLocal8Bit())}
	};
	rv.setName(name);
	rv.setValue(ProductID(PRODUCT_ID));
	rv.setTitle(QObject::tr("Файловый источник данных", name.toLocal8Bit()));

	return rv;
}
