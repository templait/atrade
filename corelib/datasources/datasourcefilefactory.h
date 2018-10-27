#pragma once

#include <datasources/datasourcefactory.h>

class DataSourceFileFactory : public DataSourceFactory::Unit
{
public:
	DataSourceFileFactory();

	// Unit interface
public:
	BDataSource *create(const DataSourceFactory::ProductID &id, const QVariantMap &settings) const;
};
