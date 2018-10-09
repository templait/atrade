#include "chartwindow.h"

#include <QAbstractSeries>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QDir> // killme

#include "chartwidget.h"
#include "datasources/datasourcefile.h" // killme
#include "datasources/datasourcequik.h" // killme
#include "series/candlestickseries.h"

#include "math.h"

using namespace QtCharts;

//return {QDateTime::currentDateTime().addDays(-200), QDateTime::currentDateTime()};

ChartWindow::ChartWindow(QWidget *parent) : QWidget(parent)
{
	mLayout = new QVBoxLayout;
	mTimeInterval = IntervalD1;
	mCandleWidth = 7;
	setLayout(mLayout);

	//mDataSource = new DataSourceQUIK(ETimeInterval::IntervalD1, "TQBR", "GAZP", "192.168.9.62", 5000, this);
	mDataSource = new DataSourceFile(QDir::homePath() + "/killme/SBER-D1.txt", this);

	CandlestickSeries* series = new CandlestickSeries;
	series->setDataSource(mDataSource);
	addSeries(series);

	mScrollBar = new QScrollBar(Qt::Horizontal);
	connect(mScrollBar, &QAbstractSlider::actionTriggered, [this](int){setScrollValue(mScrollBar->value());});
	mLayout->addWidget(mScrollBar);

	adjustScroll();
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
		mChartWidgets << widget;
	}
	else
	{	widget = mChartWidgets[widgetNum];	}

	widget->addSeries(series);
}

QDateTime ChartWindow::timeFrame() const
{
	QDateTime rv;
	if(mChartWidgets.size()>0)
	{
		qreal width = mChartWidgets[0]->plotArea().width();
		qreal candleCount = width/mCandleWidth;
		rv = QDateTime::fromSecsSinceEpoch(secsInInterval()*candleCount);
	}
	return rv;
}

qreal ChartWindow::secsInInterval() const
{
	qreal rv = NAN;
	switch(mTimeInterval)
	{
	case IntervalM1:
		rv = 60;
		break;
	case IntervalM5:
		rv = 60*5;
		break;
	case IntervalM10:
		rv = 60*10;
		break;
	case IntervalM15:
		rv = 60*15;
		break;
	case IntervalM20:
		rv = 60*20;
		break;
	case IntervalM30:
		rv = 60*30;
		break;
	case IntervalH1:
		rv = 60*60;
		break;
	case IntervalH2:
		rv = 60*60*2;
		break;
	case IntervalH4:
		rv = 60*60*4;
		break;
	case IntervalD1:
		rv = 60*60*24;
		break;
	case IntervalW1:
		rv = 60*60*24*7;
		break;
	case IntervalMN1:
		rv = 60*60*24*DAYS_IN_YEAR/12;
		break;
	default:
		rv = NAN;
	}
	return rv;
}

void ChartWindow::setViewTimeRange(TimeRange range)
{
	for(ChartWidget *w : mChartWidgets)
	{
		w->setViewTimeRange(range);
	}
}

TimeRange ChartWindow::seriesTimeRange() const
{
	TimeRange rv;
	for(ChartWidget *w : mChartWidgets)
	{
		TimeRange wRange = w->seriesTimeRange();
		if(rv.first.isNull() || rv.first > wRange.first)
		{	rv.first = wRange.first;	}
		if(rv.second.isNull() || rv.second < wRange.second)
		{	rv.second = wRange.second;	}
	}
	return rv;
}

void ChartWindow::adjustScroll()
{
	TimeRange range = seriesTimeRange();
	if(! (range.first.isNull() || range.second.isNull()))
	{
		mScrollBar->setMaximum(range.second.toSecsSinceEpoch());
		mScrollBar->setMinimum(range.first.toSecsSinceEpoch()+timeFrame().toSecsSinceEpoch());
		mScrollBar->setSingleStep(secsInInterval());
		mScrollBar->setPageStep(timeFrame().toSecsSinceEpoch());
		mScrollBar->setValue(range.second.toSecsSinceEpoch());
	}
	else
	{
		mScrollBar->setRange(0,0);
	}
}

void ChartWindow::adjustValueAxises()
{
	for(ChartWidget *w : mChartWidgets)
	{	w->adjustValueAxis();	}
}

void ChartWindow::setScrollValue(int value)
{
	QDateTime max = QDateTime::fromSecsSinceEpoch(value);
	QDateTime min = QDateTime::fromSecsSinceEpoch(value - timeFrame().toSecsSinceEpoch());
	setViewTimeRange({min, max});
	adjustValueAxises();
}

void ChartWindow::resizeEvent(QResizeEvent *event)
{
	mScrollBar->setPageStep(timeFrame().toSecsSinceEpoch());
	setScrollValue(mScrollBar->value());
	QWidget::resizeEvent(event);
}

void ChartWindow::showEvent(QShowEvent *event)
{
	mScrollBar->setPageStep(timeFrame().toSecsSinceEpoch());
	setScrollValue(mScrollBar->value());
	QWidget::showEvent(event);
}

