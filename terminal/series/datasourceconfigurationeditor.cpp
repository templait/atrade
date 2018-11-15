#include "datasourceconfigurationeditor.h"
#include <configuration.h>
#include <QColorDialog>
#include <ui_datasourceconfigurationeditor.h>

#define DECREASING_ALPHA 150

DataSourceConfigurationEditor::DataSourceConfigurationEditor(const QModelIndex &configuration, QWidget *parent)
    : ProductConfigurationEditor(configuration, parent)
{
	ui = new Ui::DataSourceConfigurationEditor;
	ui->setupUi(this);
	loadConfiguration();
	saveConfiguration();

	connect(ui->tbIncreasingColor, &QAbstractButton::clicked, this, &DataSourceConfigurationEditor::onButtonClicked);
	connect(ui->tbDecreasingColor, &QAbstractButton::clicked, this, &DataSourceConfigurationEditor::onButtonClicked);
	connect(ui->tbPenColor, &QAbstractButton::clicked, this, &DataSourceConfigurationEditor::onButtonClicked);

	connect(ui->pbSetAllColors, &QAbstractButton::clicked, this, &DataSourceConfigurationEditor::onSetAllColors);
}

DataSourceConfigurationEditor::~DataSourceConfigurationEditor()
{
	delete ui;
}

void DataSourceConfigurationEditor::loadConfiguration()
{
	QColor decreasingColor(Qt::blue);
	QColor increasingColor = decreasingColor;
	increasingColor.setAlpha(DECREASING_ALPHA);
	QColor penColor = Qt::black;

	if(configuration().containsChild("increasingColor"))
	{	increasingColor = configuration()["increasingColor"].value().value<QColor>();}
	if(configuration().containsChild("decreasingColor"))
	{	decreasingColor = configuration()["decreasingColor"].value().value<QColor>();}
	if(configuration().containsChild("penColor"))
	{	penColor = configuration()["penColor"].value().value<QColor>();}

	ui->tbDecreasingColor->setPalette(QPalette(decreasingColor));
	ui->tbIncreasingColor->setPalette(QPalette(increasingColor));
	ui->tbPenColor->setPalette(QPalette(penColor));
}

void DataSourceConfigurationEditor::saveConfiguration()
{
	if(! configuration().containsChild("increasingColor"))
	{	configuration().insertChild({"increasingColor", QVariant(), tr("Increasing color")});	}
	if(! configuration().containsChild("decreasingColor"))
	{	configuration().insertChild({"decreasingColor", QVariant(), tr("Decreasing color")});	}
	if(! configuration().containsChild("penColor"))
	{	configuration().insertChild({"penColor", QVariant(), tr("Pen color")});	}

	configuration()["increasingColor"].setValue(ui->tbIncreasingColor->palette().button().color());
	configuration()["decreasingColor"].setValue(ui->tbDecreasingColor->palette().button().color());
	configuration()["penColor"].setValue(ui->tbPenColor->palette().button().color());
}

void DataSourceConfigurationEditor::onButtonClicked()
{
	QToolButton* button = qobject_cast<QToolButton*>(sender());
	Q_ASSERT(button);
	QColor color = button->palette().button().color();
	color = QColorDialog::getColor(color, nullptr, QString(), QColorDialog::ShowAlphaChannel);
	if(color.isValid())
	{
		button->setPalette(color);
		saveConfiguration();
	}
}

void DataSourceConfigurationEditor::onSetAllColors()
{
	QColor color = ui->tbDecreasingColor->palette().button().color();
	color = QColorDialog::getColor(color, nullptr, QString(), QColorDialog::ShowAlphaChannel);
	if(color.isValid())
	{
		ui->tbDecreasingColor->setPalette(color);
		color.setAlpha(DECREASING_ALPHA);
		ui->tbIncreasingColor->setPalette(color);
		ui->tbPenColor->setPalette(Qt::black);
		saveConfiguration();
	}
}
