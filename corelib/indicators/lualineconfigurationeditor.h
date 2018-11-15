#pragma once

#include <configurationeditormodule.h>

namespace Ui
{
class LuaLineConfigurationEditor;
}

class LuaLineConfigurationEditor : public ConfigurationEditorModule
{
	Q_OBJECT
public:
	LuaLineConfigurationEditor(const QModelIndex &configuration, QWidget* parent=nullptr);
	~LuaLineConfigurationEditor();
private:
	Ui::LuaLineConfigurationEditor* ui;
};
