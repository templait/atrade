#include "quikdatasourcefactory.h"
#include "quikdatasource.h"

#include <log.h>

#include <QSettings>

#define PRODUCT_ID "c647314d-d9c0-4e97-9d57-d3c473d727ae"

QuikDataSourceFactory::QuikDataSourceFactory()
	: DataSourceFactory::Unit(QString("QUIK DataSource"), QUuid(PRODUCT_ID))
{

}

BDataSource *QuikDataSourceFactory::create(const DataSourceConf &conf) const
{
	BDataSource* rv=nullptr;
/*
	if(conf.value().toUuid() == QUuid(PRODUCT_ID))
	{		
		QSettings appSettings;
		QString quikURL = appSettings.value("DataSourceQUIK").toString();
		QStringList lst = quikURL.split(':');
		if(lst.size()==2)
		{
			QString hostName = lst[0].trimmed();
			quint16 port = static_cast<quint16>(lst[1].trimmed().toUInt());
			ETimeInterval interval = static_cast<ETimeInterval>(conf["interval"].value().toInt());
			QString className = conf["class"].value().toString();
			QString code = conf["code"].value().toString();

			rv = new QuikDataSource(interval, className, code, hostName, port);
		}
		else
		{
			Log::error(QString("%1.invalid config section DataSourceQUIK: \"%2\"").arg(__CLASS_NAME__).arg(quikURL));
		}
	}
	else
	{
		Log::error(QString("%1.invalid ProductID: \"%2\"").arg(__CLASS_NAME__).arg(conf.value().toUuid().toString()));
	}
*/
	return rv;
}

DataSourceConf *QuikDataSourceFactory::createDefaultConf() const
{
	/*
	QString name = productName();
	Configuration rv
	{
		{Configuration::Value, "class",		"TQBR",			QObject::tr("Класс",		name.toLocal8Bit())},
		{Configuration::Value, "code",		"SBER",			QObject::tr("Символ",	name.toLocal8Bit())},
		//{Configuration::Value, "interval",	IntervalD1,		QObject::tr("Интервал",		name.toLocal8Bit())}
	};
	rv.setName(name);
	rv.setValue(ProductID(PRODUCT_ID));
	rv.setTitle(QObject::tr("Источник данных QUIK", name.toLocal8Bit()));
	rv.setUserEditableMap(Configuration::Title);
*/
	return nullptr;
}
