#include "chartwidget.h"

#include <math.h>

#include "series/datasourceseries.h"

#include <QChart>
#include <QDateTimeAxis>
#include <QGraphicsLinearLayout>
#include <QValueAxis>

using namespace QtCharts;

ChartWidget::ChartWidget(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);

	mChart = new QChart;
	layout->addItem(mChart);

	mValueAxis = new QValueAxis;
	mTimeAxis = new QDateTimeAxis;
	//mTimeAxis->setFormat("dd");
	connect(mTimeAxis, SIGNAL(rangeChanged(QDateTime,QDateTime)), SLOT(onTimeRangeChanged(QDateTime,QDateTime)));
	mChart->addAxis(mValueAxis, Qt::AlignLeft);
	mChart->addAxis(mTimeAxis, Qt::AlignBottom);
}
/*
void ChartWidget::addSeries(BSeriesEx *series)
{
	QAbstractSeries* s = series->abstractSeries();
	mChart->addSeries(s);
	s->attachAxis(mValueAxis);
	s->attachAxis(mTimeAxis);
	adjustValueAxis();
	connect(s, SIGNAL(countChanged()), SLOT(onCountChanged()));

	//mChart->legend()->setVisible(true);
	mChart->legend()->setAlignment(Qt::AlignBottom);
}
*/

void ChartWidget::addDataSource(BDataSource *dataSource)
{
	DataSourceSeries *dss = new DataSourceSeries(mChart, dataSource, this);
	connect(dss, SIGNAL(candlesAdppended(int)), SLOT(onCandlesAppended(int)));
	mSeries << dss;
}

void ChartWidget::adjustValueAxis()
{
	qreal min = NAN;
	qreal max = NAN;
	for(const DataSourceSeries* series  : mSeries)
	{
		ValueRange seriesRange = series->valueRange();
		if(qIsNaN(min) || min > seriesRange.first)
		{	min = seriesRange.first;	}
		if(qIsNaN(max) || max < seriesRange.second)
		{	max = seriesRange.second;	}
	}
	if(! (qIsNaN(min) || qIsNaN(max)) )
	{	mValueAxis->setRange(min, max);	}
}

QValueAxis *ChartWidget::valueAxis()
{
	return mValueAxis;
}

QDateTimeAxis *ChartWidget::timeAxis()
{
	return mTimeAxis;
}

void ChartWidget::onCandlesAppended(int count)
{
	adjustValueAxis();
	DataSourceSeries* series = qobject_cast<DataSourceSeries*>(sender());
	Q_ASSERT(series);
	emit candlesAppended(series->dataSource(), count);
}

TimeRange ChartWidget::viewTimeRange() const
{
	return {mTimeAxis->min(), mTimeAxis->max()};
}

void ChartWidget::setViewTimeRange(const TimeRange & range)
{
	mTimeAxis->setRange(range.first.addMSecs(-1), range.second.addMSecs(1)); // Полный бред, но без добавления этих миллисекунд ось не отображается
	adjustValueAxis();
}

TimeRange ChartWidget::timeRange() const
{
	TimeRange rv;
	for(const DataSourceSeries* series  : mSeries)
	{
		TimeRange seriesRange = series->timeRange();
		if(rv.first.isNull() || rv.first > seriesRange.first)
		{	rv.first = seriesRange.first;	}
		if(rv.second.isNull() || rv.second < seriesRange.second)
		{	rv.second = seriesRange.second;	}
	}

	return rv;
}

QRectF ChartWidget::plotArea() const
{
	return mChart->plotArea();
}

void ChartWidget::onTimeRangeChanged(QDateTime min, QDateTime max)
{
	for(DataSourceSeries* series  : mSeries)
	{
		series->setViewTimeRange({min, max});
	}
}
