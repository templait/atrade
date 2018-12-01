#pragma once

#include <datasources/datasourcefactory.h>

class QuikDataSourceFactory : public DataSourceFactory::Unit
{
public:
	QuikDataSourceFactory();
	virtual ~QuikDataSourceFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const ProductConf &conf) const override;
	virtual ProductConf* createDefaultConf() const override;
};
