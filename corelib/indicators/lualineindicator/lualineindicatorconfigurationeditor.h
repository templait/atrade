#pragma once

#include <configurationeditormodule.h>

namespace Ui
{
class LuaLineIndicatorConfigurationEditor;
}

class LuaLineIndicatorConfigurationEditor : public ConfigurationEditorModule
{
	Q_OBJECT
public:
	LuaLineIndicatorConfigurationEditor(const QModelIndex &configuration, QWidget* parent=nullptr);
	~LuaLineIndicatorConfigurationEditor();
private:
	Ui::LuaLineIndicatorConfigurationEditor* ui;
};
