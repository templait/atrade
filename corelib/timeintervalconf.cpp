#include "timeintervalconf.h"

TimeIntervalConf::TimeIntervalConf(BConf *parent) : BConf(parent)
{

}

ETimeInterval TimeIntervalConf::timeInterval() const
{
	return mTimeInterval;
}

void TimeIntervalConf::setTimeInterval(ETimeInterval interval)
{
	mTimeInterval = interval;
	emit timeIntervalChanged(mTimeInterval);
}
