#include "timeintervalconf.h"

#include <QDataStream>

TimeIntervalConf::TimeIntervalConf(const QString &name) : BConf(name)
{

}

TimeIntervalConf::TimeIntervalConf() : BConf("TimeInterval")
{

}

ETimeInterval TimeIntervalConf::timeInterval() const
{
	return mTimeInterval;
}

void TimeIntervalConf::setTimeInterval(ETimeInterval interval)
{
	mTimeInterval = interval;
}

bool TimeIntervalConf::isSame(const BConf &other) const
{
	bool rv = false;
	if(const TimeIntervalConf* timeInterval = dynamic_cast<const TimeIntervalConf*>(&other))
	{
		rv = BConf::isSame(other) && mTimeInterval==timeInterval->mTimeInterval;
	}
	return rv;
}
