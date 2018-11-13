#include "mainwindow.h"
#include "logdoc/logdoc.h"
#include "chartwindow.h"
#include "configurationeditor.h"
#include "mdiarea.h"

#include <ui_mainwindow.h>

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSettings>

MainWindow::MainWindow()
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	connect(ui->actionNew_chart_window, &QAction::triggered, this, &MainWindow::onNewChartWindow);
	connect(ui->actionChart_window_configuration, &QAction::triggered, this, &MainWindow::onChartWindowConfiguration);
	connect(ui->actionNew_tab, &QAction::triggered, this, &MainWindow::onNewTab);
	connect(ui->actionCascade, &QAction::triggered, [this](){currentMDIArea()->cascadeSubWindows();});
	connect(ui->actionTile, &QAction::triggered, [this](){currentMDIArea()->tileSubWindows();});

	connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onCurrentTabChanfed);
	connect(ui->tabWidget, &QTabWidget::tabCloseRequested, ui->tabWidget, &QTabWidget::removeTab);

	initDocks();
	loadWindowState();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initDocks()
{
	mLogDoc = new LogDoc(this);
	addDockWidget(Qt::BottomDockWidgetArea, mLogDoc);
	mLogDoc->hide();// hide by default
	ui->menuView->addAction(mLogDoc->toggleViewAction());
}


void MainWindow::saveWindowState() const
{
	QSettings settings;
	settings.beginGroup(objectName());
	settings.setValue("Geometry", saveGeometry());
	settings.setValue("WindowState", saveState());

	settings.beginWriteArray("Tabs");
	for(int i=0; i<ui->tabWidget->count(); i++)
	{
		settings.setArrayIndex(i);
		QWidget* widget = ui->tabWidget->widget(i);
		settings.setValue("TabName", widget->objectName());
		if(MDIArea* mdiArea = qobject_cast<MDIArea*>(widget))
		{
			mdiArea->saveWindowState(settings);
		}
	}
	settings.endArray();
	settings.endGroup();
}

void MainWindow::loadWindowState()
{
	QSettings settings;
	settings.beginGroup(objectName());
	restoreGeometry(settings.value("Geometry").toByteArray());
	restoreState(settings.value("WindowState").toByteArray());
	int tabsCount = settings.beginReadArray("Tabs");
	for(int i=0; i<tabsCount; i++)
	{
		settings.setArrayIndex(i);
		QString tabName = settings.value("TabName").toString();
		MDIArea* mdiArea = new MDIArea;
		mdiArea->setObjectName(tabName);
		mdiArea->loadWindowState(settings);
		appendMDIArea(mdiArea);
	}
	settings.endArray();
	settings.endGroup();
}

MDIArea *MainWindow::currentMDIArea()
{
	MDIArea * rv = nullptr;
	if(MDIArea* area = qobject_cast<MDIArea*>(ui->tabWidget->currentWidget()))
	{	rv = area;	}
	return rv;
}

void MainWindow::appendMDIArea(MDIArea *area)
{
	ui->tabWidget->addTab(area, area->windowTitle());
	connect(area, &QMdiArea::subWindowActivated, this, [this](QMdiSubWindow *window){
		ui->actionChart_window_configuration->setEnabled(window);
	});
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	saveWindowState();
	QMainWindow::closeEvent(event);
}

void MainWindow::onNewChartWindow()
{
	ConfigurationEditor editor(ChartWindow::defaultConfiguration(), this);
	if(editor.exec())
	{
		ChartWindow* cw = new ChartWindow(editor.configuration());
		currentMDIArea()->addSubWindow(cw);
		cw->show();
	}
}

void MainWindow::onChartWindowConfiguration()
{
	ChartWindow* cw = qobject_cast<ChartWindow*>(currentMDIArea()->activeSubWindow()->widget());
	Q_ASSERT(cw);
	ConfigurationEditor editor(cw->configuration(), this);
	if(editor.exec())
	{	cw->loadConfiguration(editor.configuration());	}
}

void MainWindow::onNewTab()
{
	auto checkTabName = [this](const QString& name)
	{
		bool rv = true;
		for(int i=0; i<ui->tabWidget->count(); i++)
		{
			if(ui->tabWidget->widget(i)->objectName()==name)
			{
				rv = false;
				break;
			}
		}
		return rv;
	};
	int suffix=0;
	QString tabName;
	do
	{
		suffix++;
		tabName = QString("Page %1").arg(suffix);
	}
	while(!checkTabName(tabName));

	MDIArea* mdiArea = new MDIArea;
	mdiArea->setObjectName(tabName);
	mdiArea->setWindowTitle(tabName);

	appendMDIArea(mdiArea);
}

void MainWindow::onCurrentTabChanfed(int index)
{
	ui->actionNew_chart_window->setEnabled(index>=0);
	ui->actionCascade->setEnabled(index>=0);
	ui->actionTile->setEnabled(index>=0);
}

