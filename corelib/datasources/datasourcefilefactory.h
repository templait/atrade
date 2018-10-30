#pragma once

#include <datasources/datasourcefactory.h>

class DataSourceFileFactory : public DataSourceFactory::Unit
{
public:
	DataSourceFileFactory();
	virtual ~DataSourceFileFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const ProductID &id, const Configuration &settings) const override;
};
