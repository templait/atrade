#include "bseriesex.h"
#include <QAbstractSeries>

using namespace QtCharts;

BSeriesEx::BSeriesEx(BDataSource *dataSource) : mDataSource(dataSource)
{

}

BSeriesEx::~BSeriesEx()
{

}

QAbstractSeries* BSeriesEx::abstractSeries()
{
	return dynamic_cast<QAbstractSeries*>(this);
}

const QAbstractSeries* BSeriesEx::abstractSeries() const
{
	return dynamic_cast<const QAbstractSeries*>(this);
}

BDataSource *BSeriesEx::dataSource() const
{
	return mDataSource;
}

BSeriesEx *BSeriesEx::interface(QAbstractSeries *series)
{
	return dynamic_cast<BSeriesEx*>(series);
}

const BSeriesEx *BSeriesEx::interface(const QAbstractSeries *series)
{
	return dynamic_cast<const BSeriesEx*>(series);
}
