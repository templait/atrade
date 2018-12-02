#pragma once

#include <datasources/datasourcefactory.h>

class QuikDataSourceFactory : public DataSourceFactory::Unit
{
public:
	QuikDataSourceFactory();
	virtual ~QuikDataSourceFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const DataSourceConf &conf) const override;
	virtual DataSourceConf* createDefaultConf() const override;
};
