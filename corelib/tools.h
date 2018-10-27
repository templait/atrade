#pragma once

#include <types.h>

qint64 secsInInterval(ETimeInterval interval);
QString intervalToString(ETimeInterval interval);

inline QString className(const QString& prettyFunction)
{
	return prettyFunction.split("::").first();
}

#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)
