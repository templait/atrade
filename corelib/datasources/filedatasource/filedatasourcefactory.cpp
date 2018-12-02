#include "filedatasourcefactory.h"
#include "filedatasource.h"

#include <datasources/datasourceconf.h>
#include <timeintervalconf.h>
#include <log.h>
#include <tools.h>

#include <QSettings>

#define PRODUCT_ID "ab38fe10-d502-11e8-b568-0800200c9a66"

FileDataSourceFactory::FileDataSourceFactory()
	: DataSourceFactory::Unit(QString("File DataSource"), QUuid(PRODUCT_ID))
{

}

BDataSource *FileDataSourceFactory::create(const DataSourceConf &conf) const
{
	BDataSource* rv=nullptr;

	if(const DataSourceConf* dsConf = dynamic_cast<const DataSourceConf*>(&conf))
	{
		if(const TimeIntervalConf* intervalConf = conf.findParent<TimeIntervalConf>())
		{
			if(dsConf->productID() == QUuid(PRODUCT_ID))
			{
				QSettings appSettings;
				QString path = QString("%1/%2/%3/%4.txt")
				        .arg(appSettings.value("DataSourceFileDir", "../DataSourceFile").toString())
				        .arg(dsConf->param("className").toString())
				        .arg(dsConf->param("code").toString())
				        .arg(intervalToString(intervalConf->timeInterval()));
				rv = new FileDataSource(path);
			}
			else
			{	Log::error(QString("%1.invalid ProductID: \"%2\"").arg(__CLASS_NAME__).arg(conf.productID().toString()));	}
		}
		else
		{	Log::error(QString("%1.Configuration haven't TimeIntervalConf parents"));	}
	}
	else
	{	Log::error(QString("%1.Configuration isn't DataSourceConf"));	}

	return rv;
}

DataSourceConf *FileDataSourceFactory::createDefaultConf() const
{
	DataSourceConf* rv = new DataSourceConf;
	rv->setProductID(PRODUCT_ID);
	rv->setTitle(QObject::tr("Файловый источник данных", __CLASS_NAME__.toLocal8Bit()));
	rv->setParam("className", "TQBR");
	rv->setParam("code", "SBER");
	return rv;
}
