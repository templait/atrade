#include "bseriesex.h"
#include <QAbstractSeries>

using namespace QtCharts;

QAbstractSeries* BSeriesEx::abstractSeries()
{
	return dynamic_cast<QAbstractSeries*>(this);
}

const QAbstractSeries* BSeriesEx::abstractSeries() const
{
	return dynamic_cast<const QAbstractSeries*>(this);
}

BSeriesEx *BSeriesEx::interface(QAbstractSeries *series)
{
	return dynamic_cast<BSeriesEx*>(series);
}

const BSeriesEx *BSeriesEx::interface(const QAbstractSeries *series)
{
	return dynamic_cast<const BSeriesEx*>(series);
}
