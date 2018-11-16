#include "fileconfigurationeditor.h"
#include <QDir>
#include <QSettings>
#include <log.h>
#include <tools.h>
#include <ui_fileconfigurationeditor.h>
#include <configurationmodel.h>

FileConfigurationEditor::FileConfigurationEditor(const QModelIndex &index, QWidget *parent)
	: ConfigurationEditorModule(index, parent)
{
	ui = new Ui::ConfigurationEditor;
	ui->setupUi(this);

	QSettings settings;
	QDir dir(settings.value("DataSourceFileDir").toString());
	if(dir.isReadable())
	{
		dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		for(const QString& className : dir.entryList())
		{	ui->cbClass->addItem(className);	}

		if(configuration().containsChild("class"))
		{
			QString confClass = configuration()["class"].value().toString();
			int cur = ui->cbClass->findText(confClass);
			if(cur>=0)
			{
				ui->cbClass->setCurrentIndex(cur);
				loadCodesForClass(ui->cbClass->currentText());
				if(configuration().containsChild("code"))
				{
					QString confCode = configuration()["code"].value().toString();
					cur = ui->cbCode->findText(confCode);
					if(cur>=0)
					{	ui->cbCode->setCurrentIndex(cur);	}
					else
					{	Log::error(QString("%1.can't find symbol code: \"%2\"").arg(__CLASS_NAME__).arg(confCode));	}
				}
			}
			else
			{	Log::error(QString("%1.can't find class: \"%2\"").arg(__CLASS_NAME__).arg(confClass));	}
		}
		connect(ui->cbClass, qOverload<const QString&>(&QComboBox::currentIndexChanged), this, &FileConfigurationEditor::onClassActivated);
		connect(ui->cbCode, qOverload<const QString&>(&QComboBox::currentIndexChanged), this, &FileConfigurationEditor::onCodeActivated);
		connect(ui->pbSetDefaultTitle, &QAbstractButton::clicked, this, &FileConfigurationEditor::onSetDefaulTitle);
	}
	else
	{	Log::error(QString("%1.invalid path: \"%2\"").arg(__CLASS_NAME__).arg(dir.path()));	}

}

FileConfigurationEditor::~FileConfigurationEditor()
{
	delete ui;
}

bool FileConfigurationEditor::loadCodesForClass(const QString& textClassName)
{
	bool rv = false;
	ui->cbCode->clear();
	QSettings settings;
	QDir dir(settings.value("DataSourceFileDir").toString() + "/" + textClassName);
	if(dir.isReadable())
	{
		dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		for(const QString& codeName : dir.entryList())
		{	ui->cbCode->addItem(codeName);	}
		rv = true;
	}
	else
	{	Log::error(QString("%1.invalid path: \"%2\"").arg(__CLASS_NAME__).arg(dir.path()));	}
	return rv;
}

void FileConfigurationEditor::onClassActivated(const QString &textClassName)
{
	configuration()["class"].setValue(textClassName);
	loadCodesForClass(textClassName);
}

void FileConfigurationEditor::onCodeActivated(const QString &textCodeName)
{
	configuration()["code"].setValue(textCodeName);
}

void FileConfigurationEditor::onSetDefaulTitle()
{
	model()->setData(modelIndex().sibling(modelIndex().row(), 0), ui->cbClass->currentText() + '.' + ui->cbCode->currentText(), Qt::EditRole);
}
