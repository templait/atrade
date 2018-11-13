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
	FileConfigurationEditor(Configuration* configuration, QWidget* parent=nullptr);
	~FileConfigurationEditor();
private:
	void onClassActivated(const QString& textClassName);
	void onCodeActivated(const QString& textCodeName);
	Ui::ConfigurationEditor* ui;
};
