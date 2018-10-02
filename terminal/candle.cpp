#include "candle.h"

Candle::Candle(double high, double low, double open, double close, double volume, const QDateTime &time) :
	mHigh(high),
	mLow(low),
	mOpen(open),
	mClose(close),
	mVolume(volume),
	mTime(time)
{

}

Candle::Candle(){}

bool Candle::isNull() const
{
	return mTime.isNull();
}

double Candle::high() const
{
	return mHigh;
}

double Candle::low() const
{
	return mLow;
}

double Candle::open() const
{
	return mOpen;
}

double Candle::close() const
{
	return mClose;
}

double Candle::volume() const
{
	return mVolume;
}

const QDateTime &Candle::time() const
{
	return mTime;
}
