#include "titleconfeditor.h"
#include <bconf.h>
#include <ui_titleconfeditor.h>

TitleConfEditor::TitleConfEditor(BConf &conf, QWidget *parent)
	:ConfEditorModule(conf, parent)
{
	ui = new Ui::TitleConfEditor;
	ui->setupUi(this);
	ui->leTitle->setText(ConfEditorModule::conf().title());
	connect(ui->leTitle, &QLineEdit::textChanged, [this](const QString& title){
		ConfEditorModule::conf().setTitle(title);
		emit confChanged();
	});
}

TitleConfEditor::~TitleConfEditor()
{
	delete ui;
}
