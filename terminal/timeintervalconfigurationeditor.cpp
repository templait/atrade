#include "timeintervalconfigurationeditor.h"
#include <QtGlobal>
#include "ui_timeintervalconfigurationeditor.h"
/*
TimeIntervalConfigurationEditor::TimeIntervalConfigurationEditor(const QModelIndex &index, QWidget *parent)
	: ConfEditorModule(index, parent)
{
	ui = new Ui::TimeIntervalConfigurationEditor;
	ui->setupUi(this);
	ui->cbTimeInterval->setTimeInterval(static_cast<ETimeInterval>(configuration().value().toInt()));

	connect(ui->cbTimeInterval, qOverload<int>(&QComboBox::currentIndexChanged), [this](int){
		configuration().setValue(ui->cbTimeInterval->timeInterval());
	});
}

TimeIntervalConfigurationEditor::~TimeIntervalConfigurationEditor()
{
	delete ui;
}
*/
