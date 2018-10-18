#pragma once

#include <serial.hpp>
#include <point.h>
#include <QObject>

class BIndicator : public QObject, public Serial<Point>
{
	Q_OBJECT
public:
	BIndicator(QObject* parent=0) : QObject(parent){}
	virtual ~BIndicator(){}
signals:
	void pointsAppended(int count);
	void pointUpdated(int index);
};
