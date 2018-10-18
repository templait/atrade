#pragma once

#include "types.h"

class Candle
{
public:
	Candle(double high, double low, double open, double close, double volume, const QDateTime & time, ETimeInterval interval);
	Candle();

	bool isNull() const;

	double high() const;
	double low() const;
	double open() const;
	double close() const;
	double volume() const;
	const QDateTime& time() const;
	ETimeInterval interval() const;
	qint64 secsInterval() const;
private:
	double mHigh;
	double mLow;
	double mOpen;
	double mClose;
	double mVolume;
	QDateTime mTime;
	ETimeInterval mInterval;
};
