#pragma once

#include <QDateTime>
#include <QUuid>
#include <sharedpointer.hpp>
#include <QtCore/QMetaType>

enum ETimeInterval
{
	IntervalTICK,
	IntervalM1, IntervalM5,	IntervalM10, IntervalM15, IntervalM20, IntervalM30,
	IntervalH1, IntervalH2, IntervalH4,
	IntervalD1,
	IntervalW1,
	IntervalMN1,
	IntervalUnknown
};

Q_DECLARE_METATYPE(ETimeInterval);

template <class T>
struct Range : public QPair<T, T>
{
	using QPair<T, T>::QPair;
	bool isInRange(const T& value) const
	{	return value >= this->first && value <= this->second;	}
};

struct TimeRange : public Range<QDateTime>
{
	using Range<QDateTime>::Range;
};

struct ValueRange : public Range<qreal>
{
	using Range<qreal>::Range;
};

typedef QUuid ProductID;
typedef QList<QPair<QString, ProductID> > ProductList;

#define DAYS_IN_YEAR 365.259641
