#include "configurationeditor.h"

#include <ui_configurationeditor.h>

#include <QMenu>
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
	ui->tvConf->expandAll();
	connect(ui->tvConf, &QWidget::customContextMenuRequested, [this](const QPoint& point){execContextMenu(ui->tvConf->viewport()->mapToGlobal(point));});
}

ConfigurationEditor::~ConfigurationEditor()
{
	delete ui;
}

void ConfigurationEditor::execContextMenu(const QPoint &point)
{
	QModelIndex index = ui->tvConf->currentIndex();
	if(index.isValid())
	{
		QMenu menu;
		BConf* conf = mConfModel->conf(index);
		QStringList childNames = conf->childTypesNames();
		for(int i=0; i<childNames.count(); i++)
		{
			QAction* act = menu.addAction(childNames[i]);
			act->setData(i);
		}
		if(QAction* act = menu.exec(point))
		{	mConfModel->appendChild(index, act->data().toInt());	}
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
