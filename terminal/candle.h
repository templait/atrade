#pragma once

#include <QDateTime>

class Candle
{
public:
	Candle(double high, double low, double open, double close, double volume, const QDateTime & time);
	Candle();

	bool isNull() const;

	double high() const;
	double low() const;
	double open() const;
	double close() const;
	double volume() const;
	const QDateTime& time() const;
private:
	double mHigh;
	double mLow;
	double mOpen;
	double mClose;
	double mVolume;
	QDateTime mTime;
};
