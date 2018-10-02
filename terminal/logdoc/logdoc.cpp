#include "logdoc.h"

#include <log.h>

#include <ui_logdoc.h>

#include "logmodel.h"
#include <QtDebug>
LogDoc::LogDoc(QWidget *parent) : QDockWidget(parent)
{
	setWidget(new QWidget(this));
	ui = new Ui::wLogDoc;
	ui->setupUi(widget());
	setObjectName(widget()->objectName() + "_Doc");
	setWindowTitle(widget()->windowTitle());

	mLogModel = new LogModel(this);
	ui->lvLog->setModel(mLogModel);
	connect(mLogModel, &LogModel::rowsInserted, [this](QModelIndex i, int, int l){
		ui->lvLog->scrollTo(mLogModel->index(l,0, i));
	});
	connect(ui->pbClear, SIGNAL(clicked(bool)), mLogModel, SLOT(clear()));

	Log::setLogFunction([this](const QString& str, Log::ELevel l){mLogModel->appendString(str, l);});
	Log::setColor(Qt::black, Log::Info);
	Log::setColor(Qt::darkYellow, Log::Warning);
	Log::setColor(Qt::darkRed, Log::Error);
}

LogDoc::~LogDoc()
{
	delete ui;
}
