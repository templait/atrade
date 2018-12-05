#include "timeintervalconfeditor.h"
#include <QtGlobal>
#include <timeintervalconf.h>
#include "ui_timeintervalconfeditor.h"

TimeIntervalConfEditor::TimeIntervalConfEditor(TimeIntervalConf &conf, QWidget *parent)
	: ConfEditorModule(conf, parent)
{
	ui = new Ui::TimeIntervalConfEditor;
	ui->setupUi(this);
	ui->cbTimeInterval->setTimeInterval(static_cast<ETimeInterval>(conf.timeInterval()));

	connect(ui->cbTimeInterval, qOverload<int>(&QComboBox::currentIndexChanged), [this](int){
		static_cast<TimeIntervalConf&>(this->conf()).setTimeInterval(ui->cbTimeInterval->timeInterval());
	});
}

TimeIntervalConfEditor::~TimeIntervalConfEditor()
{
	delete ui;
}
