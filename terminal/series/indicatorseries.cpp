#include "indicatorseries.h"

#include <QChart>

#include "indicators/bindicator.h"

#include <QLineSeries>
#include <math.h>

using namespace QtCharts;

IndicatorSeries::IndicatorSeries(QChart *chart, BIndicator *indicator, QObject *parent)
	: QObject(parent)
	, mChart(chart)
	, mIndicator(indicator)
{
	connect(mIndicator, SIGNAL(pointsAppended(int)), SLOT(onPointsAppended(int)));
	connect(mIndicator, SIGNAL(pointUpdated(int)), SLOT(onPointUpdated(int)));
}

void IndicatorSeries::onPointsAppended(int count)
{

}

void IndicatorSeries::onPointUpdated(int index)
{

}

void IndicatorSeries::setViewTimeRange(const TimeRange &range)
{
	mViewTimeRange = range;
	qDeleteAll(mSeries);
	mSeries.clear();

	auto addSeries = [this](const QList<QPointF> & list)
	{
		QLineSeries* series = new QLineSeries;
		series->setPen({Qt::red, 1});
		mChart->addSeries(series);
		series->attachAxis(mChart->axisX());
		series->attachAxis(mChart->axisY());
		series->append(list);
		mSeries << series;
	};

	auto points = mIndicator->getTimeRange(mViewTimeRange);
	if(points.size())
	{
		QList<QPointF> list;
		for(const Point* point : points)
		{
			QPointF p(point->time().toMSecsSinceEpoch(), point->value());
			if(point->isNaN())	// обнаружен разрыв в графике. завершаем серию.
			{
				if(list.size())
				{
					addSeries(list);
					list.clear();
				}
			}
			else
			{	list << p;	}
		}
		addSeries(list);
	}
}

ValueRange IndicatorSeries::valueRange() const
{
	ValueRange rv(NAN, NAN);

	for(const Point* point : mIndicator->getTimeRange(mViewTimeRange))
	{
		if(qIsNaN(rv.first) || point->value() < rv.first)
		{	rv.first = point->value();		}
		if(qIsNaN(rv.second) || point->value() > rv.second)
		{	rv.second = point->value();	}
	}

	return rv;
}

TimeRange IndicatorSeries::timeRange() const
{
	TimeRange rv;
	if(mIndicator->size()>0)
	{
		rv = {mIndicator->first().time(), mIndicator->last().time()};
	}
	return rv;
}
