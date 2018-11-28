#include "timeintervalconf.h"

#include <QDataStream>

TimeIntervalConf::TimeIntervalConf()
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

void TimeIntervalConf::serialize(QDataStream &out) const
{
	BConf::serialize(out);
	out << mTimeInterval;
}

void TimeIntervalConf::deserialize(QDataStream &in)
{
	BConf::deserialize(in);
	ETimeInterval t;
	in >> (int&)(t);
	setTimeInterval(t);
}
