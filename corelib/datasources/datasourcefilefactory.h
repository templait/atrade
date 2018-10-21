#pragma once

#include <datasources/datasourcefactory.h>

class DataSourceFileFactory : public DataSourceFactory::Unit
{
public:
	DataSourceFileFactory();

	// Unit interface
public:
	BDataSource *create(const DataSourceFactory::ProductID &, const QVariantMap &) const;
};
