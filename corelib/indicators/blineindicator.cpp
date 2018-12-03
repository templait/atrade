#include "blineindicator.h"

#include <datasources/bdatasource.h>

#include <math.h>

#include <QTimer>

BLineIndicator::BLineIndicator(DataSource dataSource, QObject *parent)
	: BIndicator(parent)
	, mDataSource(dataSource)
{
	connect(&dataSource, SIGNAL(candlesAppended(int)), SLOT(onCandlesAppended(int)));
	connect(&dataSource, SIGNAL(candleUpdated(int)), SLOT(onCandleUpdated(int)));
	//QTimer::singleShot(100, Qt::VeryCoarseTimer, [this](){append();});
}

int BLineIndicator::size() const
{
	return mPoints.size();
}

void BLineIndicator::populate()
{
	append();
}

const Point *BLineIndicator::at(int index) const
{
	const Point * rv(nullptr);
	if(index<mPoints.size())
	{	rv = &mPoints[index]; }
	return rv;
}

DataSource BLineIndicator::dataSource() const
{
	return mDataSource;
}

void BLineIndicator::append(int start)
{
	int count=0;
	for(int i = start; i<mDataSource->size(); ++i)
	{
		const Candle* candle = mDataSource->at(i);
		if(mPoints.size() && (candle->time().toSecsSinceEpoch() - mPoints.last().time().toSecsSinceEpoch() > candle->secsInterval()))
		{
			mPoints << Point(candle->time());
			count++;
		}
		Q_ASSERT(mIndexMap.size()==i);
		mIndexMap << mPoints.size();
		mPoints << candle2point(i);
		count++;
	}
	if(count>0)
	{	emit pointsAppended(count);	}
}

void BLineIndicator::onCandlesAppended(int count)
{
	append(mDataSource->size()-count);
}

void BLineIndicator::onCandleUpdated(int index)
{
	int i = mIndexMap[index];
	Q_ASSERT(i<mPoints.size());
	Q_ASSERT(index<mDataSource->size());
	mPoints[i] = candle2point(index);
	emit pointUpdated(i);
}
