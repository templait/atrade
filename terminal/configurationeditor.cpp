#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QSettings>
#include <productlistmodel.h>

#include "confmodel.h"

ConfigurationEditor::ConfigurationEditor(BConf *conf, QWidget *parent)
	: QDialog(parent)
	, mConf(conf)
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

	mConfModel = new ConfModel(conf, this);
	ui->tvConf->setModel(mConfModel);
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
