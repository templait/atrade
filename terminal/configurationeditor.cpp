#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QSettings>
#include <tools.h>
#include <productlistmodel.h>

#include "configurationmodel.h"
#include "productconfigurationeditor.h"
#include "indicators/indicatorfactory.h"

ConfigurationEditor::ConfigurationEditor(const Configuration &configuration, QWidget *parent)
	: QDialog(parent)
	, mConfiguration(configuration)
	, mConfigurationEditor(nullptr)
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

	mConfigurationModel = new ConfigurationModel(configuration);
	ui->tvConfiguration->setModel(mConfigurationModel);
	//ui->tvConfiguration->setRootIndex(mConfigurationModel->index(0,0, QModelIndex()));
	ui->tvConfiguration->expandAll();

	connect(ui->tvConfiguration, &QAbstractItemView::activated, this, &ConfigurationEditor::onItemActivated);
}

ConfigurationEditor::~ConfigurationEditor()
{
	delete ui;
}

const Configuration &ConfigurationEditor::configuration() const
{
	return mConfigurationModel->configuration();
}

void ConfigurationEditor::setConfidurationEditor(ProductConfigurationEditor *configurationEditor)
{
	if(mConfigurationEditor)
	{	delete mConfigurationEditor;	}
	if(configurationEditor)
	{	ui->gbConfigurationEditor->layout()->addWidget(configurationEditor);	}
	mConfigurationEditor = configurationEditor;
}

void ConfigurationEditor::onItemActivated(const QModelIndex &index)
{
	QUuid uuid = index.sibling(index.row(), 1).data().toUuid();
	if(IndicatorFactory::instance().hasProduct(uuid))
	{
		setConfidurationEditor(IndicatorFactory::instance().createConfigurationEditor(uuid, mConfigurationModel->configuration(index)));
	}
	else
	{	setConfidurationEditor(nullptr);	}
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
