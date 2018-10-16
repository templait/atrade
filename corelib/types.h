#pragma once

#include <QDateTime>

enum ETimeInterval
{
	IntervalUnknown,
	IntervalTICK,
	IntervalM1, IntervalM5,	IntervalM10, IntervalM15, IntervalM20, IntervalM30,
	IntervalH1, IntervalH2, IntervalH4,
	IntervalD1,
	IntervalW1,
	IntervalMN1
};

typedef QPair<QDateTime, QDateTime> TimeRange;
typedef QPair<qreal, qreal> ValueRange;

#define DAYS_IN_YEAR 365.259641
