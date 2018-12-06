#include "filedatasourcehelper.h"
#include <tools.h>
#include <QObject>

FileDataSourceHelper::FileDataSourceHelper(DataSourceConf *conf)
	: DataSourceConfHelper("ab38fe10-d502-11e8-b568-0800200c9a66", conf)
{

}

QString FileDataSourceHelper::className() const
{
	return getParam<QString>("className");
}

void FileDataSourceHelper::setClassName(const QString &className)
{
	setParam("className", className);
}

QString FileDataSourceHelper::code() const
{
	return getParam<QString>("code");
}

void FileDataSourceHelper::setCode(const QString &code)
{
	setParam("code", code);
}

DataSourceConf *FileDataSourceHelper::createConf()
{
	DataSourceConf* rv = new DataSourceConf;
	rv->setProductID(productID());
	rv->setTitle(QObject::tr("File datasource"));
	rv->setParam("className", "TQBR");
	rv->setParam("code", "SBER");
	return rv;
}
