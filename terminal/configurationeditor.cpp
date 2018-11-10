#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QSettings>
#include <tools.h>
#include <productlistmodel.h>
#include <indicators/indicatorfactory.h>

#include "configurationmodel.h"
#include "series/datasourceconfigurationeditor.h"


ConfigurationEditor::ConfigurationEditor(const Configuration &configuration, QWidget *parent)
	: QDialog(parent)
	, mConfiguration(configuration)
	, mConfigurationEditor(nullptr)
    , mAppearanceEditor(nullptr)
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
	setProductEditor(configurationEditor, &mConfigurationEditor);
}

void ConfigurationEditor::setAppearanceEditor(ProductConfigurationEditor *configurationEditor)
{
	setProductEditor(configurationEditor, &mAppearanceEditor);
}

void ConfigurationEditor::setProductEditor(ProductConfigurationEditor *configurationEditor, ProductConfigurationEditor **dstEditor)
{
	if(*dstEditor)
	{	delete *dstEditor;	}
	if(configurationEditor)
	{	ui->gbConfigurationEditor->layout()->addWidget(configurationEditor);	}
	*dstEditor = configurationEditor;
}

void ConfigurationEditor::onItemActivated(const QModelIndex &index)
{
	QUuid uuid = index.sibling(index.row(), 1).data().toUuid();
	Configuration* conf = const_cast<Configuration*>(&(mConfigurationModel->configuration(index)));
	if(IndicatorFactory::instance().hasProduct(uuid))
	{
		setConfidurationEditor(IndicatorFactory::instance().createConfigurationEditor(uuid, conf));
		setAppearanceEditor(nullptr);
	}
	else if(DataSourceFactory::instance().hasProduct(uuid))
	{
		setConfidurationEditor(nullptr);
		setAppearanceEditor(new DataSourceConfigurationEditor(conf));
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
