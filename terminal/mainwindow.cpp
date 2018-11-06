#include "mainwindow.h"
#include "logdoc/logdoc.h"
#include "chartwindow.h"
#include "configurationeditor.h"

#include <ui_mainwindow.h>

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSettings>

MainWindow::MainWindow()
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	mMdiArea = new QMdiArea(this);
	setCentralWidget(mMdiArea);

	initDocks();
	loadWindowState();

	connect(ui->actionCreate_chart, &QAction::triggered, this, &MainWindow::createChart);
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

void MainWindow::createChart()
{
	ConfigurationEditor editor(ChartWindow::defaultConfiguration(), this);
	if(editor.exec())
	{
		ChartWindow* cw = new ChartWindow(editor.configuration());
		mMdiArea->addSubWindow(cw);
		cw->show();
	}
}

void MainWindow::saveWindowState() const
{
	QSettings settings;
	settings.beginGroup(objectName());
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());

	settings.beginWriteArray("SubWindows");
	QList<QMdiSubWindow *> subs = mMdiArea->subWindowList();
	int i=0;
	for(const QMdiSubWindow * sub : subs)
	{
		settings.setArrayIndex(i++);
		if(const ChartWindow* cw = qobject_cast<const ChartWindow*>(sub->widget()))
		{
			settings.setValue("WindowType", "ChartWindow");
			settings.setValue("geometry", sub->saveGeometry());
			cw->saveConfiguration(settings);
		}
	}
	settings.endArray();
	settings.endGroup();
}

void MainWindow::loadWindowState()
{
	QSettings settings;
	settings.beginGroup(objectName());
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());
	int chartWinCount = settings.beginReadArray("SubWindows");
	for(int i=0; i<chartWinCount; i++)
	{
		settings.setArrayIndex(i);
		QString windowType = settings.value("WindowType").toString();
		if(windowType == "ChartWindow")
		{
			ChartWindow* cw = new ChartWindow;
			QMdiSubWindow* sub = mMdiArea->addSubWindow(cw);
			sub->restoreGeometry(settings.value("geometry").toByteArray());
			cw->show();
			cw->loadConfiguration(settings);
		}
	}
	settings.endArray();
	settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	saveWindowState();
	QMainWindow::closeEvent(event);
}

