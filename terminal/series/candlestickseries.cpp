#include "candlestickseries.h"
#include "datasources/bdatasource.h"
#include "candle.h"
#include <cmath>

#include <QCandlestickSet>

using namespace QtCharts;

CandlestickSeries::CandlestickSeries(BDataSource* dataSource, QObject *parent) : QCandlestickSeries(parent), BSeriesEx(dataSource)
{
	setPen({Qt::black, 1});

	if(dataSource->size()>0)
	{	onCandlesAppended(dataSource->size());	}
	connect(dataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppended(int)));
	connect(dataSource, SIGNAL(candleUpdated(int)), SLOT(onCandleUpdated(int)));
}

ValueRange CandlestickSeries::valueRange(const QDateTime &start, const QDateTime &end) const
{
	ValueRange rv(NAN, NAN);

	for(QCandlestickSet *set : sets())
	{
		if(set->timestamp() >= start.toMSecsSinceEpoch() && set->timestamp() <= end.toMSecsSinceEpoch())
		{
			if(qIsNaN(rv.first) || set->low() < rv.first)
			{	rv.first = set->low();		}
			if(qIsNaN(rv.second) || set->high() > rv.second)
			{	rv.second = set->high();	}
		}
	}

	return rv;
}

TimeRange CandlestickSeries::timeRange() const
{
	TimeRange rv;
	if(dataSource()->size()>0)
	{
		rv = {dataSource()->first().time(), dataSource()->last().time()};
	}
	return rv;
}

void CandlestickSeries::setViewTimeRange(const TimeRange &range)
{
	mTimeRange = range;
	clear();
	QList<QCandlestickSet*> sets;

	for(const Candle& c : dataSource()->getTimeRange(range))
	{	sets << createSet(c);	}

	append(sets);
}

QCandlestickSet *CandlestickSeries::createSet(const Candle &candle) const
{
	return new QCandlestickSet(candle.open(), candle.high(), candle.low(), candle.close(), candle.time().toMSecsSinceEpoch());
}

bool CandlestickSeries::isCandleInTimeRange(const Candle &candle) const
{
	return candle.time() >= mTimeRange.first && candle.time() <= mTimeRange.second;
}

void CandlestickSeries::onCandlesAppended(int count)
{
	BDataSource& ds = *dataSource();
	QList<QCandlestickSet*> sets;
	for(int i=ds.size()-count; i<ds.size(); i++)
	{
		if(isCandleInTimeRange(ds[i]))
		sets << createSet(ds[i]);
	}
	append(sets);
}

void CandlestickSeries::onCandleUpdated(int index)
{
	BDataSource& ds = *dataSource();
	if(index < ds.size() && isCandleInTimeRange(ds[index]))
	{
		for(QCandlestickSet* set : sets())
		{
			if(ds[index].time().toMSecsSinceEpoch() == set->timestamp())
			{
				set->setOpen(ds[index].open());
				set->setClose(ds[index].close());
				set->setHigh(ds[index].high());
				set->setLow(ds[index].low());
				set->setTimestamp(ds[index].time().toMSecsSinceEpoch());
			}
		}
	}
}

