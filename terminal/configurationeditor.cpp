#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QMenu>
#include <QSettings>
#include <productlistmodel.h>

#include "confmodel.h"

ConfigurationEditor::ConfigurationEditor(const ChartWindowConf &conf, QWidget *parent)
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

	mConfModel = new ConfModel(&mConf, this);
	ui->tvConf->setModel(mConfModel);
	ui->tvConf->expandAll();
	connect(ui->tvConf, &QWidget::customContextMenuRequested, [this](const QPoint& point){execContextMenu(ui->tvConf->viewport()->mapToGlobal(point));});
}

ConfigurationEditor::~ConfigurationEditor()
{
	delete ui;
}

const ChartWindowConf &ConfigurationEditor::conf() const
{
	return mConf;
}

void ConfigurationEditor::execContextMenu(const QPoint &point)
{
	QModelIndex index = ui->tvConf->currentIndex();
	if(index.isValid())
	{
		QMenu menu;
		BConf* conf = mConfModel->conf(index);
		QAction* act = nullptr;
		QString childName = conf->creatableChildName();
		if(! childName.isEmpty())
		{	act = menu.addAction(childName);	}
		if(menu.exec(point) == act)
		{	mConfModel->appendChild(index);	}
	}
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
