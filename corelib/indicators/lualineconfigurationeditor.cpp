#include "lualineconfigurationeditor.h"
#include "ui_lualineconfigurationeditor.h"

LuaLineConfigurationEditor::LuaLineConfigurationEditor(const QModelIndex& configuration, QWidget *parent)
	:ProductConfigurationEditor(configuration, parent)
{
	ui = new Ui::LuaLineConfigurationEditor;
	ui->setupUi(this);
}

LuaLineConfigurationEditor::~LuaLineConfigurationEditor()
{
	delete ui;
}
