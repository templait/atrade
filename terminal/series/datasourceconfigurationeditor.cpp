#include "datasourceconfigurationeditor.h"

#include <QColorDialog>
#include <ui_datasourceconfigurationeditor.h>

DataSourceConfigurationEditor::DataSourceConfigurationEditor(Configuration* configuration, QWidget *parent)
    : ProductConfigurationEditor(configuration, parent)
{
	ui = new Ui::DataSourceConfigurationEditor;
	ui->setupUi(this);
	loadConfiguration();
	saveConfiguration();

	connect(ui->tbIncreasingColor, &QAbstractButton::clicked, this, &DataSourceConfigurationEditor::onButtonClicked);
	connect(ui->tbDecreasingColor, &QAbstractButton::clicked, this, &DataSourceConfigurationEditor::onButtonClicked);
	connect(ui->tbPenColor, &QAbstractButton::clicked, this, &DataSourceConfigurationEditor::onButtonClicked);
}

DataSourceConfigurationEditor::~DataSourceConfigurationEditor()
{
	delete ui;
}

void DataSourceConfigurationEditor::loadConfiguration()
{
	QColor increasingColor(Qt::blue);
	QColor decreasingColor = increasingColor;
	decreasingColor.setAlpha(decreasingColor.alpha()/2);
	QColor penColor = increasingColor;

	Configuration& conf(*configuration());
	if(conf.containsChild("increasingColor"))
	{	increasingColor = conf["increasingColor"].value().value<QColor>();}
	if(conf.containsChild("decreasingColor"))
	{	decreasingColor = conf["decreasingColor"].value().value<QColor>();}
	if(conf.containsChild("penColor"))
	{	penColor = conf["penColor"].value().value<QColor>();}

	ui->tbDecreasingColor->setPalette(QPalette(decreasingColor));
	ui->tbIncreasingColor->setPalette(QPalette(increasingColor));
	ui->tbPenColor->setPalette(QPalette(penColor));
}

void DataSourceConfigurationEditor::saveConfiguration()
{
	Configuration& conf(*configuration());
	if(! conf.containsChild("increasingColor"))
	{	conf.appendChild({"increasingColor", QVariant(), tr("Increasing color")});	}
	if(! conf.containsChild("decreasingColor"))
	{	conf.appendChild({"decreasingColor", QVariant(), tr("Decreasing color")});	}
	if(! conf.containsChild("penColor"))
	{	conf.appendChild({"penColor", QVariant(), tr("Pen color")});	}

	conf["increasingColor"].setValue(ui->tbIncreasingColor->palette().button().color());
	conf["decreasingColor"].setValue(ui->tbDecreasingColor->palette().button().color());
	conf["penColor"].setValue(ui->tbPenColor->palette().button().color());
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
