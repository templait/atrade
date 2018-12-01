#include "filedatasourcefactory.h"
#include "filedatasource.h"
#include "filedatasourceconf.h"

#include <timeintervalconf.h>
#include <log.h>
#include <tools.h>

#include <QSettings>

#define PRODUCT_ID "ab38fe10-d502-11e8-b568-0800200c9a66"

FileDataSourceFactory::FileDataSourceFactory()
	: DataSourceFactory::Unit(QString("File DataSource"), QUuid(PRODUCT_ID))
{

}

BDataSource *FileDataSourceFactory::create(const ProductConf &conf) const
{
	BDataSource* rv=nullptr;

	if(const FileDataSourceConf* dsConf = dynamic_cast<const FileDataSourceConf*>(&conf))
	{
		if(const TimeIntervalConf* intervalConf = conf.findParent<TimeIntervalConf>())
		{
			if(dsConf->productID() == QUuid(PRODUCT_ID))
			{
				QSettings appSettings;
				QString path = QString("%1/%2/%3/%4.txt")
				        .arg(appSettings.value("DataSourceFileDir", "../DataSourceFile").toString())
				        .arg(dsConf->className())
				        .arg(dsConf->code())
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
	{	Log::error(QString("%1.Configuration isn't FileDataSourceConf"));	}

	return rv;
}

ProductConf* FileDataSourceFactory::createDefaultConf() const
{
	FileDataSourceConf* rv = new FileDataSourceConf(ProductID(PRODUCT_ID), "TQBR", "SBER");
	rv->setTitle(QObject::tr("Файловый источник данных", __CLASS_NAME__.toLocal8Bit()));
	return rv;
}
