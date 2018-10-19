#include "candleadapterindicator.h"

#include <datasources/bdatasource.h>

#include <math.h>

CandleAdapterIndicator::CandleAdapterIndicator(const BDataSource *dataSource, TOtputType type, QObject *parent)
	: BLineIndicator(dataSource, parent)
	, mOutputType(type)
{
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

