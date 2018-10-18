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
	populate();
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

void CandleAdapterIndicator::populate()
{
	mPoints.clear();
	//std::transform(mDataSource->begin(), mDataSource->end(), std::back_insert_iterator<QList<Point> >(mPoints), [this](const Candle& candle){return candle2point(candle);});
	for(auto  candleIt = mDataSource->begin(); candleIt<mDataSource->end(); ++candleIt)
	{
		if(mPoints.size())
		{
			if(candleIt->time().toSecsSinceEpoch() - mPoints.last().time().toSecsSinceEpoch() > candleIt->secsInterval())
			{
				mPoints << Point(candleIt->time());
			}
		}
		mPoints << candle2point(*candleIt);
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
	std::transform(mDataSource->end()-count, mDataSource->end(), std::back_insert_iterator<QList<Point> >(mPoints), [this](const Candle& candle){return candle2point(candle);});
	emit pointsAppended(count);
}

void CandleAdapterIndicator::onCandleUpdated(int index)
{
	Q_ASSERT(index<mPoints.size());
	Q_ASSERT(index<mDataSource->size());
	mPoints[index] = candle2point(*(mDataSource->at(index)));
	emit pointUpdated(index);
}
