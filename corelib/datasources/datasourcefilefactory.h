#pragma once

#include <datasources/datasourcefactory.h>

class DataSourceFileFactory : public DataSourceFactory::Unit
{
public:
	DataSourceFileFactory();
	virtual ~DataSourceFileFactory(){}

	// Unit interface
public:
	virtual BDataSource *create(const DataSourceFactory::ProductID &id, const Configuration &settings) const override;
};
