#pragma once

#include <productconfigurationeditor.h>

namespace Ui
{
class LuaLineConfigurationEditor;
}

class LuaLineConfigurationEditor : public ProductConfigurationEditor
{
	Q_OBJECT
public:
	LuaLineConfigurationEditor(Configuration *configuration, QWidget* parent=nullptr);
	~LuaLineConfigurationEditor();
private:
	Ui::LuaLineConfigurationEditor* ui;
};
