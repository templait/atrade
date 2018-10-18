#include "point.h"

#include <math.h>

Point::Point(double value, const QDateTime &time) : mValue(value), mTime(time)
{

}

Point::Point(const QDateTime &time) : mValue(NAN), mTime(time)
{

}

Point::Point() : mValue(NAN)
{

}

bool Point::isNull() const
{
	return mTime.isNull();
}

bool Point::isNaN() const
{
	return qIsNaN(mValue);
}

double Point::value() const
{
	return mValue;
}

const QDateTime &Point::time() const
{
	return mTime;
}
