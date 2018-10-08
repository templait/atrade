#include "chartwindow.h"

#include <QAbstractSeries>
#include <QScrollBar>
#include <QVBoxLayout>

#include "chartwidget.h"
#include "datasources/datasourcequik.h"
#include "series/candlestickseries.h"

using namespace QtCharts;

ChartWindow::ChartWindow(QWidget *parent) : QWidget(parent)
{
	mLayout = new QVBoxLayout;
	setLayout(mLayout);

	mDataSource = new DataSourceQUIK(ETimeInterval::M15, "TQBR", "GAZP", "192.168.9.63", 5000, this);

	CandlestickSeries* series = new CandlestickSeries;
	series->setDataSource(mDataSource);
	addSeries(series);

	mScrollBar = new QScrollBar(Qt::Horizontal);
	mLayout->addWidget(mScrollBar);

	setMinimumSize(1000,700);
}

ChartWindow::~ChartWindow()
{
}

void ChartWindow::addSeries(BSeriesEx *series, int widgetNum)
{
	ChartWidget* widget = 0;

	if(widgetNum >= mChartWidgets.size())
	{
		widget = new ChartWidget;
		mLayout->insertWidget(widgetNum, widget);
	}
	else
	{	widget = mChartWidgets[widgetNum];	}

	widget->addSeries(series);
}

