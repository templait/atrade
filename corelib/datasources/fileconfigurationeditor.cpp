#include "fileconfigurationeditor.h"
#include <ui_fileconfigurationeditor.h>

FileConfigurationEditor::FileConfigurationEditor(Configuration *configuration, QWidget *parent)
	: ProductConfigurationEditor(configuration, parent)
{
	ui = new Ui::ConfigurationEditor;
	ui->setupUi(this);
}

FileConfigurationEditor::~FileConfigurationEditor()
{
	delete ui;
}
