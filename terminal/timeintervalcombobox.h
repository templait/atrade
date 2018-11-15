#pragma once

#include <QComboBox>

#include <types.h>

class TimeIntervalComboBox : public QComboBox
{
	Q_OBJECT
public:
	TimeIntervalComboBox(QWidget *parent = nullptr);
	void setTimeInterval(ETimeInterval timeInterval);
	ETimeInterval timeInterval() const;
};
