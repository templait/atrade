#pragma once

#include <QObject>
#include <types.h>
#include <candle.h>
#include <serial.hpp>

class BDataSource : public QObject, public Serial<Candle>
{
	Q_OBJECT
public:
	BDataSource(QObject* parent=nullptr) : QObject(parent) {}

	virtual bool isActive() const = 0;
	virtual QString errorString() const = 0;
	virtual ETimeInterval interval() const = 0;
	virtual ~BDataSource(){}

signals:
	void candlesAppended(int count);
	void candleUpdated(int index);
};
