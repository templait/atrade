#include "filedatasourceconfigurationeditor.h"
#include <QDir>
#include <QSettings>
#include <log.h>
#include <tools.h>
#include <ui_filedatasourceconfigurationeditor.h>
#include <configurationmodel.h>
#include "filedatasourceconfnames.h"

FileDataSourceConfigurationEditor::FileDataSourceConfigurationEditor(const QModelIndex &index, QWidget *parent)
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

		if(configuration().containsChild(CN_CLASS))
		{
			QString confClass = configuration()[CN_CLASS].value().toString();
			int cur = ui->cbClass->findText(confClass);
			if(cur>=0)
			{
				ui->cbClass->setCurrentIndex(cur);
				loadCodesForClass(ui->cbClass->currentText());
				if(configuration().containsChild(CN_CODE))
				{
					QString confCode = configuration()[CN_CODE].value().toString();
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
		connect(ui->cbClass, qOverload<const QString&>(&QComboBox::currentIndexChanged), this, &FileDataSourceConfigurationEditor::onClassActivated);
		connect(ui->cbCode, qOverload<const QString&>(&QComboBox::currentIndexChanged), this, &FileDataSourceConfigurationEditor::onCodeActivated);
		connect(ui->pbSetDefaultTitle, &QAbstractButton::clicked, this, &FileDataSourceConfigurationEditor::onSetDefaulTitle);
	}
	else
	{	Log::error(QString("%1.invalid path: \"%2\"").arg(__CLASS_NAME__).arg(dir.path()));	}

}

FileDataSourceConfigurationEditor::~FileDataSourceConfigurationEditor()
{
	delete ui;
}

bool FileDataSourceConfigurationEditor::loadCodesForClass(const QString& textClassName)
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

void FileDataSourceConfigurationEditor::onClassActivated(const QString &textClassName)
{
	configuration()[CN_CLASS].setValue(textClassName);
	loadCodesForClass(textClassName);
}

void FileDataSourceConfigurationEditor::onCodeActivated(const QString &textCodeName)
{
	configuration()[CN_CODE].setValue(textCodeName);
}

void FileDataSourceConfigurationEditor::onSetDefaulTitle()
{
	model()->setData(modelIndex().sibling(modelIndex().row(), 0), ui->cbClass->currentText() + '.' + ui->cbCode->currentText(), Qt::EditRole);
}
