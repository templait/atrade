#pragma once

#include <productconfigurationeditor.h>

namespace Ui
{
class ConfigurationEditor;
}

class LuaLineConfigurationEditor : public ProductConfigurationEditor
{
	Q_OBJECT
public:
	LuaLineConfigurationEditor(const Configuration& configuration, QWidget* parent=nullptr);
	~LuaLineConfigurationEditor();
private:
	Ui::ConfigurationEditor* ui;
};
