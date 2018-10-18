#include "point.h"

#include <math.h>

Point::Point(double value, const QDateTime &time) : mValue(value), mTime(time)
{

}

Point::Point() : mValue(NAN)
{

}
