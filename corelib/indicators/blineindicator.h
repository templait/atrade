#pragma once

#include <indicators/bindicator.h>
#include <datasources/datasource.h>
#include <candle.h>

class BLineIndicator : public BIndicator
{
	Q_OBJECT
public:
	BLineIndicator(DataSource dataSource, QObject* parent=nullptr);
	virtual ~BLineIndicator(){}

	// Serial interface
	int size() const override;
	virtual void populate() override;
	const Point *at(int index) const override;
protected:
	virtual Point candle2point(int index) const = 0;
	DataSource dataSource() const;
private:
	void append(int start=0);

	DataSource mDataSource;
	QList<Point> mPoints;
	QList<int> mIndexMap;
private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);
};
