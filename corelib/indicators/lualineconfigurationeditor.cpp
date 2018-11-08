#include "lualineconfigurationeditor.h"
#include "ui_lualineconfigurationeditor.h"

LuaLineConfigurationEditor::LuaLineConfigurationEditor(const Configuration &configuration, QWidget *parent)
	:ProductConfigurationEditor(configuration, parent)
{
	ui = new Ui::ConfigurationEditor;
	ui->setupUi(this);
}

LuaLineConfigurationEditor::~LuaLineConfigurationEditor()
{
	delete ui;
}
