#include "timeintervalconf.h"

#include <QDataStream>

TimeIntervalConf::TimeIntervalConf(const QString &name) : BConf(name.isEmpty() ? "TimeInterval" : name)
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

void TimeIntervalConf::serialize(QDataStream &out) const
{
	BConf::serialize(out);
	out << mTimeInterval;
}

void TimeIntervalConf::deserialize(QDataStream &in)
{
	BConf::deserialize(in);
	in >> (int&)mTimeInterval;
}

