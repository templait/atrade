#pragma once

#include <indicators/candleadapterindicator.h>

#include <candle.h>

class MAIndicator : public CandleAdapterIndicator
{
	Q_OBJECT
public:
	enum TMAType  {SMA, EMA};

	MAIndicator(DataSource dataSource, TMAType type=SMA, TOtputType outputType=CandleAdapterIndicator::Close, QObject* parent=0);
	virtual ~MAIndicator(){}
protected:
	virtual Point candle2point(int index) const override;
private:
	TMAType mMAType;
	CandleAdapterIndicator * mCandleAdapterIndicator;
	TOtputType * mOutputType;
};
