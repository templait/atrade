#pragma once

#include <datasources/datasourcefactory.h>

class FileDataSourceFactory : public DataSourceFactory::Unit
{
public:
	FileDataSourceFactory();
	virtual ~FileDataSourceFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const DataSourceConf &conf) const override;
	virtual DataSourceConf *createDefaultConf() const override;
};
