#pragma once

#include <datasources/datasourcefactory.h>

class FileDataSourceFactory : public DataSourceFactory::Unit
{
public:
	FileDataSourceFactory();
	virtual ~FileDataSourceFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const ProductConf &conf) const override;
	virtual ProductConf *createDefaultConf() const override;
};
