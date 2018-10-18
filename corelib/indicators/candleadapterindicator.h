#pragma once

#include <indicators/bindicator.h>

#include <candle.h>

class BDataSource;

class CandleAdapterIndicator : public BIndicator
{
	Q_OBJECT
public:
	enum TOtputType  {Open, Close, High, Low, Volume, MOpenClose, MHighLow};

	CandleAdapterIndicator(const BDataSource* dataSource, TOtputType type=MOpenClose, QObject* parent=0);
	virtual ~CandleAdapterIndicator(){}

	// Serial interface
	int size() const override;
	const Point *at(int index) const override;
private:
	void populate();
	Point candle2point(const Candle& candle) const;

	const BDataSource* mDataSource;
	TOtputType mOutputType;
	QList<Point> mPoints;
private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);
};
