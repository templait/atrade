#pragma once

#include <productconfigurationeditor.h>

namespace Ui
{
class ConfigurationEditor;
}

class FileConfigurationEditor : public ProductConfigurationEditor
{
	Q_OBJECT
public:
	FileConfigurationEditor(const QModelIndex& rootConfiguration, QWidget* parent=nullptr);
	~FileConfigurationEditor();
private:
	bool loadCodesForClass(const QString& textClassName);

	void onClassActivated(const QString& textClassName);
	void onCodeActivated(const QString& textCodeName);
	void onSetDefaulTitle();

	Ui::ConfigurationEditor* ui;
};
