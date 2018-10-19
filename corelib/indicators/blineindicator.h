#pragma once

#include <indicators/bindicator.h>

#include <candle.h>

class BDataSource;

class BLineIndicator : public BIndicator
{
	Q_OBJECT
public:
	BLineIndicator(const BDataSource* dataSource, QObject* parent=0);
	virtual ~BLineIndicator(){}

	// Serial interface
	int size() const override;
	const Point *at(int index) const override;
protected:
	virtual Point candle2point(const Candle& candle) const = 0;
private:
	void append(int start=0);

	const BDataSource* mDataSource;
	QList<Point> mPoints;
	QList<int> mIndexMap;
private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);
};
