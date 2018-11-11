#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QSettings>
#include <tools.h>
#include <productlistmodel.h>
#include <indicators/indicatorfactory.h>
#include <datasources/datasourcefactory.h>

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
	ui->tvConfiguration->addAction(ui->actionNew_chart);
	ui->tvConfiguration->addAction(ui->actionDelete);

	connect(ui->tvConfiguration->selectionModel(), &QItemSelectionModel::currentChanged, this, &ConfigurationEditor::onCurrentChanged);
	connect(ui->actionNew_chart, &QAction::triggered, this, &ConfigurationEditor::onNewChart);
	connect(ui->actionDelete, &QAction::triggered, this, &ConfigurationEditor::onDelete);
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

void ConfigurationEditor::onCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
	QUuid uuid = current.sibling(current.row(), 1).data().toUuid();
	Configuration* conf = const_cast<Configuration*>(&(mConfigurationModel->configuration(current)));
	if(IndicatorFactory::instance().hasProduct(uuid))
	{
		setConfidurationEditor(IndicatorFactory::instance().createConfigurationEditor(uuid, conf));
		setAppearanceEditor(nullptr);
		ui->actionDelete->setEnabled(true);
	}
	else if(DataSourceFactory::instance().hasProduct(uuid))
	{
		setConfidurationEditor(nullptr);
		setAppearanceEditor(new DataSourceConfigurationEditor(conf));
		ui->actionDelete->setEnabled(true);
	}
	else if(conf->name() == "Chart")
	{
		setConfidurationEditor(nullptr);
		setAppearanceEditor(nullptr);
		ui->actionDelete->setEnabled(true);
	}
	else
	{
		setConfidurationEditor(nullptr);
		setAppearanceEditor(nullptr);
		ui->actionDelete->setEnabled(false);
	}
}

void ConfigurationEditor::onNewChart()
{
	mConfigurationModel->insertChild(mConfigurationModel->index(0,0, QModelIndex()), {Configuration::Title, "Chart", QVariant(), tr("График")});
}

void ConfigurationEditor::onDelete()
{
	QModelIndex index = ui->tvConfiguration->selectionModel()->currentIndex();
	mConfigurationModel->removeRow(index.row(), index.parent());
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
