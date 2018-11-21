#pragma once

#include <bconf.h>
#include <types.h>

class TimeIntervalConf : public BConf
{
	Q_OBJECT
	Q_PROPERTY(ETimeInterval timeInterval READ timeInterval WRITE setTimeInterval NOTIFY timeIntervalChanged)
public:
	TimeIntervalConf(BConf *parent);
	virtual ~TimeIntervalConf() override {}

	ETimeInterval timeInterval() const;
	void setTimeInterval(ETimeInterval interval);
private:
	ETimeInterval mTimeInterval;
signals:
	void timeIntervalChanged(ETimeInterval interval);
};
