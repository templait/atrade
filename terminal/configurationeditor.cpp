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

ConfigurationEditor::ConfigurationEditor(const Configuration &configuration, QWidget *parent)
	: QDialog(parent)
	, mConfigurationEditor(nullptr)
    , mAppearanceEditor(nullptr)
	, mTitleConfigurationEditor(nullptr)
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

	mConfigurationModel = new ConfigurationModel(configuration, this);
	ui->tvConfiguration->setModel(mConfigurationModel);
	//ui->tvConfiguration->setRootIndex(mConfigurationModel->index(0,0, QModelIndex()));
	ui->tvConfiguration->hideColumn(1);
	ui->tvConfiguration->hideColumn(2);
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

void ConfigurationEditor::setConfidurationEditor(ConfigurationEditorModule *configurationEditor)
{
	setEditorModule(configurationEditor, &mConfigurationEditor);
}

void ConfigurationEditor::setAppearanceEditor(ConfigurationEditorModule *configurationEditor)
{
	setEditorModule(configurationEditor, &mAppearanceEditor);
}

void ConfigurationEditor::setTitleEditor(ConfigurationEditorModule *configurationEditor)
{
	setEditorModule(configurationEditor, &mTitleConfigurationEditor);
}

void ConfigurationEditor::setEditorModule(ConfigurationEditorModule *configurationEditor, ConfigurationEditorModule **dstEditor)
{
	if(*dstEditor)
	{	delete *dstEditor;	}
	if(configurationEditor)
	{	ui->vlConfigurators->addWidget(configurationEditor);	}
	*dstEditor = configurationEditor;
}

void ConfigurationEditor::onCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
	QUuid uuid = current.sibling(current.row(), 1).data().toUuid();
	//Configuration* conf = const_cast<Configuration*>(&(mConfigurationModel->configuration(current)));
	QString currentName = mConfigurationModel->configuration(current).name();
	if(IndicatorFactory::instance().hasProduct(uuid))
	{
		setTitleEditor(nullptr);
		setConfidurationEditor(IndicatorFactory::instance().createConfigurationEditor(uuid, current));
		setAppearanceEditor(nullptr);
		ui->actionDelete->setEnabled(true);
	}
	else if(DataSourceFactory::instance().hasProduct(uuid))
	{
		setTitleEditor(new TitleConfigurationEditor(current));
		setConfidurationEditor(DataSourceFactory::instance().createConfigurationEditor(uuid, current));
		setAppearanceEditor(new DataSourceConfigurationEditor(current));
		ui->actionDelete->setEnabled(true);
	}
	else if(currentName == CHART_CONF)
	{
		setTitleEditor(new TitleConfigurationEditor(current));
		setConfidurationEditor(nullptr);
		setAppearanceEditor(nullptr);
		ui->actionDelete->setEnabled(true);
	}
	else if(currentName == CHART_WINDOW_CONF)
	{
		setTitleEditor(new TitleConfigurationEditor(current));
		setConfidurationEditor(nullptr);
		setAppearanceEditor(nullptr);
		ui->actionDelete->setEnabled(false);
	}
	else
	{
		setTitleEditor(nullptr);
		setConfidurationEditor(nullptr);
		setAppearanceEditor(nullptr);
		ui->actionDelete->setEnabled(false);
	}
}

void ConfigurationEditor::onNewChart()
{
	mConfigurationModel->insertChild(mConfigurationModel->index(0,0, QModelIndex()), {Configuration::Title, CHART_CONF, QVariant(), tr("График")});
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
