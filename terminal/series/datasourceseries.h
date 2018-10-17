#pragma once

#include <QCandlestickSeries>
#include <QChart>
#include <QObject>
#include <QPointer>
#include <types.h>

namespace QtCharts
{
class QChart;
class QCandlestickSeries;
class QCandlestickSet;
}

class BDataSource;
class Candle;

class DataSourceSeries : public QObject
{
	Q_OBJECT
public:
	DataSourceSeries(QtCharts::QChart* chart, BDataSource* dataSource, QObject* parent=0);
	void setViewTimeRange(const TimeRange& range);
	ValueRange valueRange() const;	//!< return value range for currenr view timerange
	TimeRange timeRange() const;	//!< return whole timerange
	const BDataSource* dataSource() const;

private:
	QtCharts::QCandlestickSet* createSet(const Candle *candle) const;
	bool isCandleInTimeRange(const Candle *candle) const;

	QPointer<QtCharts::QChart> mChart;
	BDataSource* mDataSource;
	QPointer<QtCharts::QCandlestickSeries> mCandleStickSeries;
	TimeRange mViewTimeRange;
private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);
signals:
	void candlesAdppended(int);	//!< в качестве аргумента - кол-во добавленых свечек
};
