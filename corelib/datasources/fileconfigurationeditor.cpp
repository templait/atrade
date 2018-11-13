#include "fileconfigurationeditor.h"
#include <QDir>
#include <QSettings>
#include <log.h>
#include <tools.h>
#include <ui_fileconfigurationeditor.h>


FileConfigurationEditor::FileConfigurationEditor(Configuration *configuration, QWidget *parent)
	: ProductConfigurationEditor(configuration, parent)
{
	ui = new Ui::ConfigurationEditor;
	ui->setupUi(this);

	connect(ui->cbClass, qOverload<const QString&>(&QComboBox::currentIndexChanged), this, &FileConfigurationEditor::onClassActivated);
	connect(ui->cbCode, qOverload<const QString&>(&QComboBox::currentIndexChanged), this, &FileConfigurationEditor::onCodeActivated);

	QSettings settings;
	QDir dir(settings.value("DataSourceFileDir").toString());
	if(dir.isReadable())
	{
		dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		for(const QString& className : dir.entryList())
		{	ui->cbClass->addItem(className);	}

		if(configuration->containsChild("class"))
		{
			QString confClass = (*configuration)["class"].value().toString();
			int cur = ui->cbClass->findText(confClass);
			if(cur>=0)
			{
				ui->cbClass->setCurrentIndex(cur);
				if(configuration->containsChild("code"))
				{
					QString confCode = (*configuration)["code"].value().toString();
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
	}
	else
	{	Log::error(QString("%1.invalid path: \"%2\"").arg(__CLASS_NAME__).arg(dir.path()));	}
}

FileConfigurationEditor::~FileConfigurationEditor()
{
	delete ui;
}

void FileConfigurationEditor::onClassActivated(const QString &textClassName)
{
	ui->cbCode->clear();
	QSettings settings;
	QDir dir(settings.value("DataSourceFileDir").toString() + "/" + textClassName);
	(*configuration())["class"].setValue(textClassName);
	if(dir.isReadable())
	{
		dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
		for(const QString& codeName : dir.entryList())
		{	ui->cbCode->addItem(codeName);	}

	}
	else
	{	Log::error(QString("%1.invalid path: \"%2\"").arg(__CLASS_NAME__).arg(dir.path()));	}
}

void FileConfigurationEditor::onCodeActivated(const QString &textCodeName)
{
	(*configuration())["code"].setValue(textCodeName);
}
