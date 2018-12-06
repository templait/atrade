#include "filedatasourcefactory.h"
#include "filedatasource.h"
#include "filedatasourcehelper.h"

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

	FileDataSourceHelper helper(const_cast<DataSourceConf*>(&conf));
	if(helper.isValid())
	{
		if(helper.timeIntervalConf())
		{
			QSettings appSettings;
			QString path = QString("%1/%2/%3/%4.txt")
					.arg(appSettings.value("DataSourceFileDir", "../DataSourceFile").toString())
					.arg(helper.className())
					.arg(helper.code())
					.arg(intervalToString(helper.timeIntervalConf()->timeInterval()));
			rv = new FileDataSource(path);
		}
		else
		{	Log::error(QString("%1.Configuration haven't TimeIntervalConf parents").arg(__CLASS_NAME__));	}
	}
	else
	{	Log::error(QString("%1.Configuration isn't DataSourceConf or have invalid ProductID").arg(__CLASS_NAME__));	}

	return rv;
}

DataSourceConf *FileDataSourceFactory::createDefaultConf() const
{
	FileDataSourceHelper helper;
	return helper.createConf();
}
