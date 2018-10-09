#pragma once

#include "types.h"
#include <qabstractseries.h>

namespace QtCharts
{
class QAbstractSeries;
class QAbstractSeries;
}

class BSeriesEx
{
public:
	virtual ~BSeriesEx();
	virtual ValueRange valueRange(const QDateTime& start, const QDateTime& end) const = 0;
	virtual TimeRange timeRange() const = 0;

	QtCharts::QAbstractSeries* abstractSeries();
	const QtCharts::QAbstractSeries *abstractSeries() const;

	static BSeriesEx* interface(QtCharts::QAbstractSeries* series);
	static const BSeriesEx* interface(const QtCharts::QAbstractSeries* series);
};
