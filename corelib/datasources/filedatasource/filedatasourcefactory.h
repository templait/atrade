#pragma once

#include <datasources/datasourcefactory.h>

class FileDataSourceFactory : public DataSourceFactory::Unit
{
public:
	FileDataSourceFactory();
	virtual ~FileDataSourceFactory() override{}

	// Unit interface
public:
	virtual BDataSource *create(const Configuration &configuration) const override;
	virtual Configuration defaultConfiguration() const override;
	virtual ConfigurationEditorModule *createConfigurationEditor(const QModelIndex &configuration, QWidget *parent) const override;
};
