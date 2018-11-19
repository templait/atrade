#pragma once

#include <configurationeditormodule.h>

namespace Ui
{
class ConfigurationEditor;
}

class FileDataSourceConfigurationEditor : public ConfigurationEditorModule
{
	Q_OBJECT
public:
	FileDataSourceConfigurationEditor(const QModelIndex& index, QWidget* parent=nullptr);
	~FileDataSourceConfigurationEditor();
private:
	bool loadCodesForClass(const QString& textClassName);

	void onClassActivated(const QString& textClassName);
	void onCodeActivated(const QString& textCodeName);
	void onSetDefaulTitle();

	Ui::ConfigurationEditor* ui;
};
