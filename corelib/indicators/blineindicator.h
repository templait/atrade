#pragma once

#include <indicators/bindicator.h>
#include <datasources/datasource.h>
#include <candle.h>

class BLineIndicator : public BIndicator
{
	Q_OBJECT
public:
	BLineIndicator(DataSource dataSource, QObject* parent=0);
	virtual ~BLineIndicator(){}

	// Serial interface
	int size() const override;
	const Point *at(int index) const override;
protected:
	virtual Point candle2point(const Candle& candle) const = 0;
private:
	void append(int start=0);

	DataSource mDataSource;
	QList<Point> mPoints;
	QList<int> mIndexMap;
private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);
};
