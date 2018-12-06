#include "confeditor.h"

#include <ui_confeditor.h>

#include <QMenu>
#include <QSettings>
#include <productlistmodel.h>

#include "confmodel.cpp"
#include "timeintervalconfeditor.h"
#include "titleconfeditor.h"

ConfEditor::ConfEditor(const ChartWindowConf &conf, QWidget *parent)
	: QDialog(parent)
	, mConf(conf)
{
	ui = new Ui::ConfEditor;
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
	connect(ui->tvConf->selectionModel(), &QItemSelectionModel::currentChanged, this, &ConfEditor::onCurrentConfChanged);
}

ConfEditor::~ConfEditor()
{
	delete ui;
}

const ChartWindowConf &ConfEditor::conf() const
{
	return mConf;
}

void ConfEditor::clearConfEditor()
{
	qDeleteAll(ui->gbConfModules->findChildren<ConfEditorModule*>());
}

void ConfEditor::showChartWindowConf(ChartWindowConf &conf)
{
	TitleConfEditor* tce = new TitleConfEditor(conf);
	ui->vlConfModules->addWidget(tce);
	connect(tce, &ConfEditorModule::confChanged, this, &ConfEditor::updateCurrentConf);
	ui->vlConfModules->addWidget(new TimeIntervalConfEditor(conf));
}

void ConfEditor::showChartConf(ChartConf &conf)
{
	TitleConfEditor* tce = new TitleConfEditor(conf);
	ui->vlConfModules->addWidget(tce);
	connect(tce, &ConfEditorModule::confChanged, this, &ConfEditor::updateCurrentConf);
}

void ConfEditor::updateCurrentConf()
{
	QModelIndex currentIndex = ui->tvConf->selectionModel()->currentIndex();
	Q_ASSERT(currentIndex.isValid());
	emit mConfModel->dataChanged(currentIndex, currentIndex);
}

void ConfEditor::execContextMenu(const QPoint &point)
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

void ConfEditor::onCurrentConfChanged(const QModelIndex &index)
{
	BConf* conf = mConfModel->conf(index);
	if(conf)
	{
		clearConfEditor();
		if(ChartWindowConf* cwConf = dynamic_cast<ChartWindowConf*>(conf))
		{	showChartWindowConf(*cwConf);	}
		else if(ChartConf* cConf = dynamic_cast<ChartConf*>(conf))
		{	showChartConf(*cConf);	}
	}
}

void ConfEditor::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	settings.beginGroup(objectName());
	settings.setValue("geometry", saveGeometry());
	settings.setValue("splitterState", ui->splitter->saveState());
	settings.endGroup();
	QDialog::closeEvent(event);
}
