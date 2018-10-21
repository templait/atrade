#include "blineindicator.h"

#include <datasources/bdatasource.h>

#include <math.h>

BLineIndicator::BLineIndicator(DataSource dataSource, QObject *parent)
	: BIndicator(parent)
	, mDataSource(dataSource)
{
	connect(&dataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppended(int)));
	connect(&dataSource, SIGNAL(candleUpdated(int)), SLOT(onCandleUpdated(int)));
	append();
}

int BLineIndicator::size() const
{
	return mPoints.size();
}

const Point *BLineIndicator::at(int index) const
{
	const Point * rv(0);
	if(index<mPoints.size())
	{	rv = &mPoints[index]; }
	return rv;
}

void BLineIndicator::append(int start)
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

void BLineIndicator::onCandlesAppended(int count)
{
	//std::transform(mDataSource->end()-count, mDataSource->end(), std::back_insert_iterator<QList<Point> >(mPoints), [this](const Candle& candle){return candle2point(candle);});
	append(mDataSource->size()-count);
	emit pointsAppended(count);
}

void BLineIndicator::onCandleUpdated(int index)
{
	int i = mIndexMap[index];
	Q_ASSERT(i<mPoints.size());
	Q_ASSERT(index<mDataSource->size());
	mPoints[i] = candle2point(*(mDataSource->at(index)));
	emit pointUpdated(i);
}
