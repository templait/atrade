#pragma once

#include <datasources/datasourcefactory.h>

class DataSourceQUIKFactory : public DataSourceFactory::Unit
{
public:
	DataSourceQUIKFactory();
	virtual ~DataSourceQUIKFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const Configuration &configuration) const override;
	virtual Configuration defaultConfiguration() const override;
};
