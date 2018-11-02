#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QSettings>
#include <tools.h>

#include "productlistmodel.h"
#include "configurationmodel.h"

ConfigurationEditor::ConfigurationEditor(const Configuration &configuration, QWidget *parent)
	: QDialog(parent)
	, mConfiguration(configuration)
{
	ui = new Ui::ConfigurationEditor;
	ui->setupUi(this);

	QSettings settings;
	restoreGeometry(settings.value(objectName() + "/geometry").toByteArray());
	ui->splitter->restoreState(settings.value(objectName() + "/splitterState").toByteArray());

	mProductListModel = new ProductListModel(this);
	ui->tvProductList->setModel(mProductListModel);
	ui->tvProductList->expandAll();

	mConfigurationModel = new ConfigurationModel(this);
	ui->tvConfiguration->setModel(mConfigurationModel);
	//ui->tvConfiguration->setRootIndex(mConfigurationModel->index(0,0, QModelIndex()));
	ui->tvConfiguration->expandAll();
}

ConfigurationEditor::~ConfigurationEditor()
{
	delete ui;
}

void ConfigurationEditor::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	settings.setValue(objectName() + "/geometry", saveGeometry());
	settings.setValue(objectName() + "/splitterState", ui->splitter->saveState());
	QDialog::closeEvent(event);
}
