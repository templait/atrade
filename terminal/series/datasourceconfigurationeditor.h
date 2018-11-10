#pragma once

#include <productconfigurationeditor.h>

namespace Ui
{
class DataSourceConfigurationEditor;
}

class DataSourceConfigurationEditor : public ProductConfigurationEditor
{
	Q_OBJECT
public:
	DataSourceConfigurationEditor(Configuration *configuration, QWidget* parent=nullptr);
	~DataSourceConfigurationEditor();
private:
	Ui::DataSourceConfigurationEditor* ui;

	void loadConfiguration();
	void saveConfiguration();

	void onButtonClicked();
};
