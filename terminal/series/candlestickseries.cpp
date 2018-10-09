#include "candlestickseries.h"
#include "datasources/bdatasource.h"
#include "candle.h"
#include <cmath>

#include <QCandlestickSet>

using namespace QtCharts;

CandlestickSeries::CandlestickSeries(QObject *parent) : QCandlestickSeries(parent), mDataSource(0)
{
	setPen({Qt::black, 1});
}

void CandlestickSeries::setDataSource(BDataSource *dataSource)
{
	if(mDataSource)
	{
		disconnect(mDataSource, 0, this, 0);
	}

	mDataSource = dataSource;
	clear();
	if(dataSource->size()>0)
	{	onCandlesAppended(dataSource->size());	}
	if(mDataSource)
	{
		connect(mDataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppended(int)));
	}
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
	BDataSource& ds = *mDataSource;
	QList<QCandlestickSet*> sets;
	for(int i=ds.size()-count; i<ds.size(); i++)
	{
		sets << new QCandlestickSet(ds[i].open(), ds[i].high(), ds[i].low(), ds[i].close(), ds[i].time().toMSecsSinceEpoch());
	}
	append(sets);
}

