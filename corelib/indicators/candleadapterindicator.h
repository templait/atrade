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
	Point candle2point(const Candle& candle) const;	// эту функцию нужно сделать виртуальной
private:
	TOtputType mOutputType;
};
