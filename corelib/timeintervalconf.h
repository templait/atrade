#pragma once

#include <bconf.h>
#include <types.h>

class TimeIntervalConf : public BConf
{
public:
	TimeIntervalConf(const QString& name=QString());
	virtual ~TimeIntervalConf() override {}

	ETimeInterval timeInterval() const;
	void setTimeInterval(ETimeInterval interval);
private:
	ETimeInterval mTimeInterval;

	// BConf interface
public:
	virtual bool isSame(const BConf &other) const override;
};
