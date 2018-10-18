#include "datasourceseries.h"

#include <QCandlestickSeries>
#include <QCandlestickSet>
#include <QChart>
#include <candle.h>
#include <datasources/bdatasource.h>

#include <math.h>

using namespace QtCharts;

DataSourceSeries::DataSourceSeries(QtCharts::QChart *chart, BDataSource *dataSource, QObject *parent)
	: QObject(parent)
	, mChart(chart)
	, mDataSource(dataSource)
	, mCandleStickSeries(new QCandlestickSeries)
{
	mChart->addSeries(mCandleStickSeries);
	mCandleStickSeries->attachAxis(mChart->axisX());
	mCandleStickSeries->attachAxis(mChart->axisY());
	mCandleStickSeries->setPen({Qt::black, 1});
	mCandleStickSeries->setBodyOutlineVisible(false);
	connect(mDataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppended(int)));
	connect(mDataSource, SIGNAL(candleUpdated(int)), SLOT(onCandleUpdated(int)));
}

void DataSourceSeries::setViewTimeRange(const TimeRange &range)
{
	Q_ASSERT(mCandleStickSeries);
	mViewTimeRange = range;
	mCandleStickSeries->clear();
	QList<QCandlestickSet*> sets;

	for(const Candle* c : mDataSource->getTimeRange(mViewTimeRange))
	{	sets << createSet(c);	}

	mCandleStickSeries->append(sets);
}

ValueRange DataSourceSeries::valueRange() const
{
	ValueRange rv(NAN, NAN);

	for(const Candle* candle : mDataSource->getTimeRange(mViewTimeRange))
	{
		if(qIsNaN(rv.first) || candle->low() < rv.first)
		{	rv.first = candle->low();		}
		if(qIsNaN(rv.second) || candle->high() > rv.second)
		{	rv.second = candle->high();	}
	}

	return rv;
}

TimeRange DataSourceSeries::timeRange() const
{
	TimeRange rv;
	if(mDataSource->size()>0)
	{
		rv = {mDataSource->first().time(), mDataSource->last().time()};
	}
	return rv;
}

const BDataSource *DataSourceSeries::dataSource() const
{
	return mDataSource;
}

QCandlestickSet *DataSourceSeries::createSet(const Candle *candle) const
{
	return new QCandlestickSet(candle->open(), candle->high(), candle->low(), candle->close(), candle->time().toMSecsSinceEpoch());
}

bool DataSourceSeries::isCandleInTimeRange(const Candle *candle) const
{
	return candle->time() >= mViewTimeRange.first && candle->time() <= mViewTimeRange.second;
}

void DataSourceSeries::onCandlesAppended(int count)
{
	Q_ASSERT(mCandleStickSeries);
	QList<QCandlestickSet*> sets;
	for(int i=mDataSource->size()-count; i<mDataSource->size(); i++)
	{
		if(isCandleInTimeRange(mDataSource->at(i)))
		sets << createSet(mDataSource->at(i));
	}
	mCandleStickSeries->append(sets);
	emit candlesAdppended(count);
}

void DataSourceSeries::onCandleUpdated(int index)
{
	Q_ASSERT(mCandleStickSeries);
	if(index < mDataSource->size() && isCandleInTimeRange(mDataSource->at(index)))
	{
		for(QCandlestickSet* set : mCandleStickSeries->sets())
		{
			if(mDataSource->at(index)->time().toMSecsSinceEpoch() == set->timestamp())
			{

				set->setOpen(mDataSource->at(index)->open());
				set->setClose(mDataSource->at(index)->close());
				set->setHigh(mDataSource->at(index)->high());
				set->setLow(mDataSource->at(index)->low());
				set->setTimestamp(mDataSource->at(index)->time().toMSecsSinceEpoch());

				//*set = createSet(mDataSource->at(index));
			}
		}
	}
}
