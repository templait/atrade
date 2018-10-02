#include "candlestickseries.h"
#include "bdatasource.h"
#include "candle.h"

#include <QCandlestickSet>

CandlestickSeries::CandlestickSeries(QObject *parent) : QtCharts::QCandlestickSeries(parent), mDataSource(0)
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
	if(mDataSource)
	{
		connect(mDataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppended(int)));
	}
}

void CandlestickSeries::onCandlesAppended(int count)
{
	BDataSource& ds = *mDataSource;
	QList<QtCharts::QCandlestickSet*> sets;
	for(int i=ds.size()-count; i<ds.size(); i++)
	{
		sets << new QtCharts::QCandlestickSet(ds[i].open(), ds[i].high(), ds[i].low(), ds[i].close(), ds[i].time().toMSecsSinceEpoch());
	}
	append(sets);
}

