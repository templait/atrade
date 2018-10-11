#include "chartwindow.h"

#include <QAbstractSeries>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QDateTimeAxis>
#include <QDir> // killme

#include "chartwidget.h"
#include "datasources/datasourcefile.h" // killme
#include "datasources/datasourcequik.h" // killme
#include "series/candlestickseries.h"

#include "math.h"

using namespace QtCharts;

ChartWindow::ChartWindow(QWidget *parent) : QWidget(parent)
{
	mLayout = new QVBoxLayout;
	mLayout->setSpacing(0);
	mTimeInterval = IntervalM1;
	mCandleWidth = 7;
	setLayout(mLayout);

	BDataSource *ds;
//*
	ds = new DataSourceQUIK(mTimeInterval, "TQBR", "GAZP", "192.168.9.62", 5000, this);
	addDataSource(ds, 0);
	mDataSources << ds;

	ds = new DataSourceQUIK(mTimeInterval, "TQBR", "SBER", "192.168.9.62", 5000, this);
	addDataSource(ds, 1);
	mDataSources << ds;

//*/
/*
	ds = new DataSourceFile(QDir::homePath() + "/killme/SBER-M1.txt", this);
	addDataSource(ds);
	mDataSources << ds;
*/
	mScrollBar = new QScrollBar(Qt::Horizontal);
	mScrollBar->setEnabled(false);
	connect(mScrollBar, &QAbstractSlider::actionTriggered, [this](int){
		setScrollValue(mScrollBar->sliderPosition());
	});
	mLayout->addWidget(mScrollBar);

	adjustScroll();
	setMinimumSize(1000,700);
}

ChartWindow::~ChartWindow()
{
	qDeleteAll(mDataSources);
}

void ChartWindow::addDataSource(BDataSource *dataSource, int widgetNum)
{
	ChartWidget* widget = 0;

	CandlestickSeries* series = new CandlestickSeries(dataSource);

	if(widgetNum >= mChartWidgets.size())
	{
		for(ChartWidget* w : mChartWidgets)
		{	w->timeAxis()->hide();		}
		widget = new ChartWidget;
		mLayout->insertWidget(widgetNum, widget);
		mChartWidgets << widget;
	}
	else
	{	widget = mChartWidgets[widgetNum];	}

	connect(dataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppend(int)));

	widget->addSeries(series);
}

qint64 ChartWindow::timeFrame() const
{
	qint64 rv(-1);
	if(mChartWidgets.size()>0)
	{
		qreal width = mChartWidgets[0]->plotArea().width();
		qreal candleCount = width/mCandleWidth;
		rv = secsInInterval()*candleCount;
	}
	return rv;
}

qint64 ChartWindow::secsInInterval() const
{
	qint64 rv = -1;
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
		rv = -2;
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
		mScrollBar->setMaximum(range.second.toSecsSinceEpoch()+secsInInterval());
		mScrollBar->setMinimum(range.first.toSecsSinceEpoch()+timeFrame());
		mScrollBar->setSingleStep(secsInInterval()); // нужно выровнять разницу между минимумом и максимумом по минимальному шагу
		mScrollBar->setPageStep(timeFrame());
		if(! mScrollBar->isEnabled())
		{
			mScrollBar->setEnabled(true);
			mScrollBar->setSliderPosition(mScrollBar->maximum());
		}
	}
	else
	{
		mScrollBar->setRange(0,0);
		mScrollBar->setEnabled(false);
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
	QDateTime min = QDateTime::fromSecsSinceEpoch(value - timeFrame());
	setViewTimeRange({min, max});
	adjustValueAxises();
}

void ChartWindow::onCandlesAppend(int)
{
	bool need2Last = false;
	if(mScrollBar->sliderPosition() == mScrollBar->maximum())
	{	need2Last = true;	}
	adjustScroll();
	if(need2Last)
	{	setScrollValue(mScrollBar->maximum());	}
}

void ChartWindow::resizeEvent(QResizeEvent *event)
{
	mScrollBar->setPageStep(timeFrame());
	setScrollValue(mScrollBar->sliderPosition());
	QWidget::resizeEvent(event);
}

void ChartWindow::showEvent(QShowEvent *event)
{
	mScrollBar->setPageStep(timeFrame());
	setScrollValue(mScrollBar->sliderPosition());
	QWidget::showEvent(event);
}

