#include "lualineindicatorconfigurationeditor.h"
#include "ui_lualineindicatorconfigurationeditor.h"
#include "lualineindicatorconfnames.h"

#include <datasources/datasourcefactory.h>

LuaLineIndicatorConfigurationEditor::LuaLineIndicatorConfigurationEditor(const QModelIndex& modelIndex, QWidget *parent)
	:ConfigurationEditorModule(modelIndex, parent)
{
	ui = new Ui::LuaLineIndicatorConfigurationEditor;
	ui->setupUi(this);

	connect(ui->cbProduct, qOverload<int>(&QComboBox::currentIndexChanged), this, &LuaLineIndicatorConfigurationEditor::onProductChanged);
}

LuaLineIndicatorConfigurationEditor::~LuaLineIndicatorConfigurationEditor()
{
	delete ui;
}

void LuaLineIndicatorConfigurationEditor::onProductChanged(int)
{
	ProductID id = ui->cbProduct->currentProduct().toByteArray();

	if(! configuration().containsChild(CN_SOURCE))
	{
		configuration().insertChild(DataSourceFactory::instance().defaultConfiguration(id));
	}


	//setEditorModule(DataSourceFactory::instance().createConfigurationEditor(id));
}

void LuaLineIndicatorConfigurationEditor::setEditorModule(ConfigurationEditorModule *configurationEditor)
{
	if(mConfigModule)
	{	delete mConfigModule;	}
	if(configurationEditor)
	{	ui->gbDataSource->layout()->addWidget(configurationEditor);	}
	mConfigModule = configurationEditor;
}
