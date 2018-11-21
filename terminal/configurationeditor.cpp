#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QSettings>
#include <tools.h>
#include <productlistmodel.h>
#include <indicators/indicatorfactory.h>
#include <datasources/datasourcefactory.h>

#include "configurationmodel.h"
#include "series/datasourceconfigurationeditor.h"
#include "titleconfigurationeditor.h"
#include "timeintervalconfigurationeditor.h"
#include "confnames.h"

ConfigurationEditor::ConfigurationEditor(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::ConfigurationEditor;
	ui->setupUi(this);

	QSettings settings;
	settings.beginGroup(objectName());
	restoreGeometry(settings.value("geometry").toByteArray());
	ui->splitter->restoreState(settings.value("splitterState").toByteArray());
	settings.endGroup();

	mProductListModel = new ProductListModel(this);
	ui->tvProductList->setModel(mProductListModel);
	ui->tvProductList->expandAll();


}

ConfigurationEditor::~ConfigurationEditor()
{
	delete ui;
}

void ConfigurationEditor::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	settings.beginGroup(objectName());
	settings.setValue("geometry", saveGeometry());
	settings.setValue("splitterState", ui->splitter->saveState());
	settings.endGroup();
	QDialog::closeEvent(event);
}
