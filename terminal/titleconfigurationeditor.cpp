#include "titleconfigurationeditor.h"
#include <ui_titleconfigurationeditor.h>
#include <configurationmodel.h>

TitleConfigurationEditor::TitleConfigurationEditor(const QModelIndex &index, QWidget *parent)
	:ConfigurationEditorModule(index, parent)
{
	ui = new Ui::TitleConfigurationEditor;
	ui->setupUi(this);
	ui->leTitle->setText(configuration().title());
	connect(ui->leTitle, &QLineEdit::textChanged, [this](const QString& text){
		model()->setData(modelIndex().sibling(modelIndex().row(), 0), text, Qt::EditRole);
	});
	connect(model(), &QAbstractItemModel::dataChanged, this, &TitleConfigurationEditor::onDataChanged);
}

TitleConfigurationEditor::~TitleConfigurationEditor()
{
	delete ui;
}

void TitleConfigurationEditor::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &)
{
	if(topLeft.parent()==modelIndex().parent() && topLeft.column()==0)
	{
		if(modelIndex().row()>=topLeft.row() && modelIndex().row()<=bottomRight.row())
		{
			ui->leTitle->setText(configuration().title());
		}
	}
}
