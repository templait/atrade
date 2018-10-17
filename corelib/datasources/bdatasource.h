#pragma once

#include <QObject>
#include <types.h>
#include <candle.h>

class BDataSource : public QObject
{
	Q_OBJECT
private:
	class const_iterator;
public:
	BDataSource(QObject* parent=0);

	virtual int size() const = 0;
	virtual const Candle * at(int index) const = 0; //!< временная метка каждой последующей свечки должна быть больше предыдущей.
	virtual bool isActive() const = 0;
	virtual QString errorString() const = 0;
	virtual ETimeInterval interval() const = 0;

	const Candle& operator[](int index) const;
	const Candle& first() const;
	const Candle& last() const;
	const_iterator begin() const;
	const_iterator end() const;
	QList<const Candle *> getTimeRange(const TimeRange &range) const;

	virtual ~BDataSource(){}

signals:
	void candlesAppended(int);
	void candleUpdated(int);
};
