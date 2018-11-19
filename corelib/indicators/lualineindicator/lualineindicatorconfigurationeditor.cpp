#include "lualineindicatorconfigurationeditor.h"
#include "ui_lualineindicatorconfigurationeditor.h"

LuaLineIndicatorConfigurationEditor::LuaLineIndicatorConfigurationEditor(const QModelIndex& configuration, QWidget *parent)
	:ConfigurationEditorModule(configuration, parent)
{
	ui = new Ui::LuaLineIndicatorConfigurationEditor;
	ui->setupUi(this);
}

LuaLineIndicatorConfigurationEditor::~LuaLineIndicatorConfigurationEditor()
{
	delete ui;
}
