#pragma once

#include <QObject>
#include <types.h>
#include <candle.h>

class BDataSource : public QObject
{
	Q_OBJECT
public:
	BDataSource(QObject* parent=0);

	virtual int size() const = 0;
	virtual const Candle & operator[](int index) const = 0; //!< временная метка каждой последующей свечки должна быть больше предыдущей.
	virtual bool isActive() const = 0;
	virtual QString errorString() const = 0;
	virtual ETimeInterval interval() const = 0;

	const Candle& first() const;
	const Candle& last() const;
	QList<Candle> getTimeRange(const TimeRange &range) const;

	virtual ~BDataSource(){}
signals:
	void candlesAppended(int);
	void candleUpdated(int);
};
