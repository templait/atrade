#pragma once

#include <bconf.h>
#include <types.h>

class TimeIntervalConf : public BConf
{
protected:
	TimeIntervalConf(const QString& name);
public:
	TimeIntervalConf();
	virtual ~TimeIntervalConf() override {}

	ETimeInterval timeInterval() const;
	void setTimeInterval(ETimeInterval interval);
private:
	ETimeInterval mTimeInterval;

	// BConf interface
public:
	virtual bool isSame(const BConf &other) const override;
	virtual void serialize(QDataStream &out) const override;
	virtual void deserialize(QDataStream &in) override;
};
