#pragma once

#include <bconf.h>
#include <types.h>

class TimeIntervalConf : public BConf
{
	Q_OBJECT
	Q_PROPERTY(ETimeInterval timeInterval READ timeInterval WRITE setTimeInterval NOTIFY timeIntervalChanged)
public:
	Q_INVOKABLE TimeIntervalConf();
	virtual ~TimeIntervalConf() override {}

	ETimeInterval timeInterval() const;
	void setTimeInterval(ETimeInterval interval);
private:
	ETimeInterval mTimeInterval;
signals:
	void timeIntervalChanged(ETimeInterval interval);

	// BConf interface
protected:
	virtual void serialize(QDataStream &out) const override;
	virtual void deserialize(QDataStream &in) override;
};
