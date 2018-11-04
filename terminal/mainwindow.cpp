#include "mainwindow.h"
#include "logdoc/logdoc.h"
#include "chartwindow.h"
#include "configurationeditor.h"

#include <ui_mainwindow.h>

#include <QMdiArea>
#include <QSettings>

MainWindow::MainWindow()
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);

	mMdiArea = new QMdiArea(this);
	setCentralWidget(mMdiArea);

	initDocks();

	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());

	connect(ui->actionCreate_chart, SIGNAL(triggered(bool)), SLOT(createChart()));
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
		ChartWindow* w = new ChartWindow(editor.configuration());
		mMdiArea->addSubWindow(w);
		w->show();
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(event);
}

