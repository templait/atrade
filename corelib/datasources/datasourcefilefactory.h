#pragma once

#include <datasources/datasourcefactory.h>

class DataSourceFileFactory : public DataSourceFactory::Unit
{
public:
	DataSourceFileFactory();
	virtual ~DataSourceFileFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const Configuration &configuration) const override;
	virtual Configuration defaultConfiguration() const override;
};
