#include "lualineindicatorconfigurationeditor.h"
/*
#include "ui_lualineindicatorconfigurationeditor.h"
#include "lualineindicatorconfnames.h"

#include <datasources/datasourcefactory.h>
#include <configurationmodel.h>

LuaLineIndicatorConfigurationEditor::LuaLineIndicatorConfigurationEditor(const QModelIndex& modelIndex, QWidget *parent)
	: ConfigurationEditorModule(modelIndex, parent)
	, mConfigModule(nullptr)
{
	ui = new Ui::LuaLineIndicatorConfigurationEditor;
	ui->setupUi(this);

	updateSourceEditor();

	connect(ui->cbProduct, qOverload<int>(&QComboBox::currentIndexChanged), this, &LuaLineIndicatorConfigurationEditor::onProductChanged);
}

LuaLineIndicatorConfigurationEditor::~LuaLineIndicatorConfigurationEditor()
{
	delete ui;
}

void LuaLineIndicatorConfigurationEditor::onProductChanged(int)
{
	ProductID id = ui->cbProduct->currentProduct();

	configuration()[CN_SOURCE].deleteChildren();
	if(DataSourceFactory::instance().hasProduct(id))
	{
		Configuration defaultConf = DataSourceFactory::instance().defaultConfiguration(id);
		configuration()[CN_SOURCE].insertChild(defaultConf);

		updateSourceEditor();
	}
}

void LuaLineIndicatorConfigurationEditor::setEditorModule(ConfigurationEditorModule *configurationEditor)
{
	if(mConfigModule)
	{	delete mConfigModule;	}
	if(configurationEditor)
	{	ui->gbDataSource->layout()->addWidget(configurationEditor);	}
	mConfigModule = configurationEditor;
}

void LuaLineIndicatorConfigurationEditor::updateSourceEditor()
{
	const Configuration* srcConf = configuration()[CN_SOURCE].childAt(0);
	if(srcConf)
	{
		ProductID id = srcConf->value().toUuid();
		if(DataSourceFactory::instance().hasProduct(id))
		{
			setEditorModule(DataSourceFactory::instance().createConfigurationEditor(id, sourceIndex()));
		}
	}
}

QModelIndex LuaLineIndicatorConfigurationEditor::sourceIndex()
{
	const Configuration* sourceConf = configuration()[CN_SOURCE].childAt(0);
	return model()->modelIndex(sourceConf);
}

const Configuration *LuaLineIndicatorConfigurationEditor::sourceConf() const
{
	const Configuration * rv = nullptr;
	if(configuration()[CN_SOURCE].childrenCount()==1)
	{	rv = configuration()[CN_SOURCE].childAt(0);	}
	return rv;
}
*/
