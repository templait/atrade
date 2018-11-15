#pragma once

#include <configurationeditormodule.h>

namespace Ui
{
class ConfigurationEditor;
}

class FileConfigurationEditor : public ConfigurationEditorModule
{
	Q_OBJECT
public:
	FileConfigurationEditor(const QModelIndex& index, QWidget* parent=nullptr);
	~FileConfigurationEditor();
private:
	bool loadCodesForClass(const QString& textClassName);

	void onClassActivated(const QString& textClassName);
	void onCodeActivated(const QString& textCodeName);
	void onSetDefaulTitle();

	Ui::ConfigurationEditor* ui;
};
