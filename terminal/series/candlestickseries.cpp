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
	if(sets().size())
	{
		rv = {QDateTime::fromMSecsSinceEpoch(sets().first()->timestamp()), QDateTime::fromMSecsSinceEpoch(sets().last()->timestamp())};
	}
	return rv;
}

void CandlestickSeries::onCandlesAppended(int count)
{
	BDataSource& ds = *dataSource();
	QList<QCandlestickSet*> sets;
	for(int i=ds.size()-count; i<ds.size(); i++)
	{
		sets << new QCandlestickSet(ds[i].open(), ds[i].high(), ds[i].low(), ds[i].close(), ds[i].time().toMSecsSinceEpoch());
	}
	append(sets);
}

void CandlestickSeries::onCandleUpdated(int index)
{
	auto sts = sets();
	if(index < sts.size())
	{
		QCandlestickSet* set = sts[index];
		const Candle& candle = (*dataSource())[index];
		set->setOpen(candle.open());
		set->setClose(candle.close());
		set->setHigh(candle.high());
		set->setLow(candle.low());
		set->setTimestamp(candle.time().toMSecsSinceEpoch());
	}
}

