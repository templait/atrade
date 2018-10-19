#include "lineindicatorseries.h"

#include <QChart>

#include "indicators/bindicator.h"

#include <QLineSeries>
#include <math.h>

using namespace QtCharts;

LineIndicatorSeries::LineIndicatorSeries(QChart *chart, BIndicator *indicator, QObject *parent)
	: QObject(parent)
	, mChart(chart)
	, mIndicator(indicator)
{
	connect(mIndicator, SIGNAL(pointsAppended(int)), SLOT(onPointsAppended(int)));
	connect(mIndicator, SIGNAL(pointUpdated(int)), SLOT(onPointUpdated(int)));
}

void LineIndicatorSeries::appendPoints(const QList<const Point *> points)
{
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

void LineIndicatorSeries::onPointsAppended(int count)
{
	/*
	QList<const Point *> points;
	std::transform(mIndicator->end()-count, mIndicator->end(), std::back_insert_iterator<QList<const Point *> >(points), [](const Point& p){return &p;});
	*/
	setViewTimeRange(mViewTimeRange);
}

void LineIndicatorSeries::onPointUpdated(int index)
{
	setViewTimeRange(mViewTimeRange);
}

void LineIndicatorSeries::setViewTimeRange(const TimeRange &range)
{
	mViewTimeRange = range;
	qDeleteAll(mSeries);
	mSeries.clear();

	appendPoints(mIndicator->getTimeRange(mViewTimeRange));
}

ValueRange LineIndicatorSeries::valueRange() const
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

TimeRange LineIndicatorSeries::timeRange() const
{
	TimeRange rv;
	if(mIndicator->size()>0)
	{
		rv = {mIndicator->first().time(), mIndicator->last().time()};
	}
	return rv;
}
