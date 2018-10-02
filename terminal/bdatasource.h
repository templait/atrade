#pragma once

#include <QObject>
#include <etimeinterval.h>

class Candle;

class BDataSource : public QObject
{
	Q_OBJECT
public:
	BDataSource(ETimeInterval interval, QObject* parent=0);

	void setInterval(ETimeInterval interval);
	ETimeInterval interval() const;

	virtual int size() const = 0;
	virtual const Candle & operator[](int index) const = 0;
	virtual bool isActive() const = 0;
	virtual QString errorString() const = 0;

	virtual ~BDataSource(){}
protected:
	virtual void onIntervalChanged() = 0;

private:
	ETimeInterval mInterval;
signals:
	void candlesAppended(int);
};
