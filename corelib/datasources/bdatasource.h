#pragma once

#include <QObject>
#include <types.h>
#include <candle.h>
#include <serial.hpp>

class BDataSource : public QObject, public Serial<Candle>
{
	Q_OBJECT
private:
	class const_iterator;
public:
	BDataSource(QObject* parent=0) : QObject(parent) {}

	virtual bool isActive() const = 0;
	virtual QString errorString() const = 0;
	virtual ETimeInterval interval() const = 0;
	virtual ~BDataSource(){}

signals:
	void candlesAppended(int count);
	void candleUpdated(int index);
};
