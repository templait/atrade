#include "candleadapterindicator.h"

#include <datasources/bdatasource.h>

#include <math.h>

CandleAdapterIndicator::CandleAdapterIndicator(const BDataSource *dataSource, TOtputType type, QObject *parent)
	: BIndicator(parent)
	, mDataSource(dataSource)
	, mOutputType(type)
{
	connect(dataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppended(int)));
	connect(dataSource, SIGNAL(candleUpdated(int)), SLOT(onCandleUpdated(int)));
	append();
}

int CandleAdapterIndicator::size() const
{
	return mPoints.size();
}

const Point *CandleAdapterIndicator::at(int index) const
{
	const Point * rv(0);
	if(index<mPoints.size())
	{	rv = &mPoints[index]; }
	return rv;
}

void CandleAdapterIndicator::append(int start)
{
	for(int i = start; i<mDataSource->size(); ++i)
	{
		const Candle* candle = mDataSource->at(i);
		if(mPoints.size() && (candle->time().toSecsSinceEpoch() - mPoints.last().time().toSecsSinceEpoch() > candle->secsInterval()))
		{
			mPoints << Point(candle->time());
		}
		Q_ASSERT(mIndexMap.size()==i);
		mIndexMap << mPoints.size();
		mPoints << candle2point(*candle);
	}
}

Point CandleAdapterIndicator::candle2point(const Candle &candle) const
{
	Point rv;

	switch (mOutputType) {
	case Open:
		rv = {candle.open(), candle.time()};
		break;
	case Close:
		rv = {candle.close(), candle.time()};
		break;
	case High:
		rv = {candle.high(), candle.time()};
		break;
	case Low:
		rv = {candle.low(), candle.time()};
		break;
	case Volume:
		rv = {candle.volume(), candle.time()};
		break;
	case MHighLow:
		rv = {(candle.high()+candle.low())/2, candle.time()};
		break;
	case MOpenClose:
		rv = {(candle.open()+candle.close())/2, candle.time()};
		break;
	}

	return rv;
}

void CandleAdapterIndicator::onCandlesAppended(int count)
{
	//std::transform(mDataSource->end()-count, mDataSource->end(), std::back_insert_iterator<QList<Point> >(mPoints), [this](const Candle& candle){return candle2point(candle);});
	append(mDataSource->size()-count);
	emit pointsAppended(count);
}

void CandleAdapterIndicator::onCandleUpdated(int index)
{
	int i = mIndexMap[index];
	Q_ASSERT(i<mPoints.size());
	Q_ASSERT(index<mDataSource->size());
	mPoints[i] = candle2point(*(mDataSource->at(index)));
	emit pointUpdated(i);
}
