#pragma once

#include <configurationeditormodule.h>

namespace Ui
{
class DataSourceConfigurationEditor;
}

class DataSourceConfigurationEditor : public ConfigurationEditorModule
{
	Q_OBJECT
public:
	DataSourceConfigurationEditor(const QModelIndex &index, QWidget *parent=nullptr);
	~DataSourceConfigurationEditor();
private:
	Ui::DataSourceConfigurationEditor* ui;

	void loadConfiguration();
	void saveConfiguration();

	void onButtonClicked();
	void onSetAllColors();
};
