#pragma once

#include <datasources/datasourceconfhelper.h>
#include <datasources/datasourceconf.h>

class FileDataSourceHelper : public DataSourceConfHelper
{
public:
	FileDataSourceHelper(DataSourceConf* conf=nullptr);

	QString className() const;
	void setClassName(const QString& className);
	QString code() const;
	void setCode(const QString& code);

	// ProductConfHelper interface
public:
	virtual DataSourceConf *createConf() override;
};

