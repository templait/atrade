#include "chartwindow.h"

#include <QVBoxLayout>

#include "chartwidget.h"
#include "datasourcequik.h"
#include "candlestickseries.h"

ChartWindow::ChartWindow(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	mDataSource = new DataSourceQUIK(ETimeInterval::M15, "TQBR", "GAZP", "192.168.9.68", 5000, this);

	CandlestickSeries* series = new CandlestickSeries;
	series->setDataSource(mDataSource);

	ChartWidget* chartWidget = new ChartWidget;
	chartWidget->addSeries(series);

	layout->addWidget(chartWidget);

	setMinimumSize(1000,700);
}

ChartWindow::~ChartWindow()
{
}
