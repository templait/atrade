#pragma once

#include <QObject>
#include <types.h>

class Candle;

class BDataSource : public QObject
{
	Q_OBJECT
public:
	BDataSource(QObject* parent=0);

	virtual int size() const = 0;
	virtual const Candle & operator[](int index) const = 0;
	virtual bool isActive() const = 0;
	virtual QString errorString() const = 0;
	virtual ETimeInterval interval() const = 0;

	virtual ~BDataSource(){}
signals:
	void candlesAppended(int);
};
