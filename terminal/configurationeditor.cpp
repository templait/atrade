#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include "productlistmodel.h"

ConfigurationEditor::ConfigurationEditor(const Configuration &configuration, QWidget *parent)
	: QDialog(parent)
	, mConfiguration(configuration)
{
	ui = new Ui::ConfigurationEditor;
	ui->setupUi(this);

	mProductListModel = new ProductListModel(this);
	ui->tvProductList->setModel(mProductListModel);
}

ConfigurationEditor::~ConfigurationEditor()
{
	delete ui;
}
