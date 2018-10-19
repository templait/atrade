#pragma once

#include <indicators/candleadapterindicator.h>

#include <candle.h>

class BDataSource;

class MAIndicator : public BLineIndicator
{
	Q_OBJECT
public:
	enum TMAType  {SMA, EMA};

	MAIndicator(const BDataSource* dataSource, TMAType type=SMA, CandleAdapterIndicator::TOtputType outputType=CandleAdapterIndicator::Close, QObject* parent=0);
	virtual ~MAIndicator(){}
protected:
	Point candle2point(const Candle& candle) const;	// эту функцию нужно сделать виртуальной
private:
	TMAType mMAType;
	CandleAdapterIndicator * mCandleAdapterIndicator;
	CandleAdapterIndicator::TOtputType * mOutputType;
};
