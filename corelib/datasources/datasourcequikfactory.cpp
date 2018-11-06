#include "datasourcequikfactory.h"
#include "datasourcefile.h"
#include "datasourcequik.h"

#include <log.h>

#include <QSettings>

#define PRODUCT_ID "c647314d-d9c0-4e97-9d57-d3c473d727ae"

DataSourceQUIKFactory::DataSourceQUIKFactory()
	: DataSourceFactory::Unit(QString("QUIK datasoure"), QUuid(PRODUCT_ID))
{

}

BDataSource *DataSourceQUIKFactory::create(const Configuration &configuration) const
{
	BDataSource* rv=nullptr;

	if(configuration.value().toUuid() == QUuid(PRODUCT_ID))
	{		
		QSettings appSettings;
		QString quikURL = appSettings.value("DataSourceQUIK").toString();
		QStringList lst = quikURL.split(':');
		if(lst.size()==2)
		{
			QString hostName = lst[0].trimmed();
			quint16 port = static_cast<quint16>(lst[1].trimmed().toUInt());
			ETimeInterval interval = static_cast<ETimeInterval>(configuration["interval"].value().toInt());
			QString className = configuration["class"].value().toString();
			QString code = configuration["code"].value().toString();

			rv = new DataSourceQUIK(interval, className, code, hostName, port);
		}
		else
		{
			Log::error(QString("%1.invalid config section DataSourceQUIK: \"%2\"").arg(__CLASS_NAME__).arg(quikURL));
		}
	}
	else
	{
		Log::error(QString("%1.invalid ProductID: \"%2\"").arg(__CLASS_NAME__).arg(configuration.value().toUuid().toString()));
	}

	return rv;
}

Configuration DataSourceQUIKFactory::defaultConfiguration() const
{
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

	return rv;
}
