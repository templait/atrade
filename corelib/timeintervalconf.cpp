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

void TimeIntervalConf::serialize(QDataStream &out) const
{
	BConf::serialize(out);
	out << static_cast<int>(mTimeInterval);
}

void TimeIntervalConf::deserialize(QDataStream &in)
{
	BConf::deserialize(in);
	int interval;
	in >> interval;
	mTimeInterval = static_cast<ETimeInterval>(interval);
}
