#include "chartwidget.h"

#include <math.h>

#include "series/datasourceseries.h"
#include "series/lineindicatorseries.h"

#include <QChart>
#include <QDateTimeAxis>
#include <QGraphicsLinearLayout>
#include <QValueAxis>
#include <configuration.h>
#include <datasources/datasourcefactory.h>

using namespace QtCharts;

ChartWidget::ChartWidget(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);

	mChart = new QChart;
	//mChart->setAnimationOptions(QChart::SeriesAnimations);
	layout->addItem(mChart);

	mValueAxis = new QValueAxis;
	mTimeAxis = new QDateTimeAxis;
	connect(mTimeAxis, SIGNAL(rangeChanged(QDateTime,QDateTime)), SLOT(onTimeRangeChanged(QDateTime,QDateTime)));
	mChart->addAxis(mValueAxis, Qt::AlignLeft);
	mChart->addAxis(mTimeAxis, Qt::AlignBottom);
	mChart->legend()->hide();
}

ChartWidget::ChartWidget(const Configuration &configuration, QGraphicsItem *parent)
    : ChartWidget(parent)
{
	for(int i=0; i<configuration.childrenCount(); i++)
	{
		const Configuration* conf = configuration.childAt(i);
		if(DataSourceFactory::instance().hasProduct(conf->value().toUuid()))
		{
			addDataSource(*conf);
		}
	}
}

void ChartWidget::addDataSource(const Configuration &dataSource)
{
	DataSource ds = DataSourceFactory::instance().product(dataSource);
	if(!ds.isNull())
	{
		DataSourceSeries *dss = new DataSourceSeries(mChart, ds, this);
		connect(dss, SIGNAL(candlesAdppended(int)), SLOT(onCandlesAppended(int)));
		mSeries << dss;
	}
}

void ChartWidget::addIndicator(BIndicator *indicator)
{
	LineIndicatorSeries *is = new LineIndicatorSeries(mChart, indicator, this);
	mSeries << is;
}

void ChartWidget::adjustValueAxis()
{
	qreal min = NAN;
	qreal max = NAN;
	for(const BSeries* series  : mSeries)
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
	for(const BSeries* series  : mSeries)
	{
		TimeRange seriesRange = series->timeRange();
		if(seriesRange.first.isValid())
		{
			if(rv.first.isNull() || rv.first > seriesRange.first)
			{	rv.first = seriesRange.first;	}
			if(rv.second.isNull() || rv.second < seriesRange.second)
			{	rv.second = seriesRange.second;	}
		}
	}

	return rv;
}

QRectF ChartWidget::plotArea() const
{
	return mChart->plotArea();
}

void ChartWidget::onTimeRangeChanged(QDateTime min, QDateTime max)
{
	for(BSeries* series  : mSeries)
	{
		series->setViewTimeRange({min, max});
	}
}
