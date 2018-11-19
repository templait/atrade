#pragma once

#include <indicators/blineindicator.h>

#include <candle.h>

class CandleAdapterIndicator : public BLineIndicator
{
	Q_OBJECT
public:
	enum TOtputType  {Open, Close, High, Low, Volume, MOpenClose, MHighLow};

	CandleAdapterIndicator(DataSource dataSource, TOtputType type=MOpenClose, QObject* parent=0);
	virtual ~CandleAdapterIndicator(){}
protected:
	virtual Point candle2point(int index) const override;
private:
	TOtputType mOutputType;
};
