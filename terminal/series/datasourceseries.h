#pragma once

#include <QCandlestickSeries>
#include <QChart>
#include <QObject>
#include <QPointer>

#include <datasources/datasource.h>

#include "bseries.h"

namespace QtCharts
{
class QChart;
class QCandlestickSeries;
class QCandlestickSet;
}

class Candle;

class DataSourceSeries : public QObject, public BSeries
{
	Q_OBJECT
public:
	DataSourceSeries(QtCharts::QChart* chart, DataSource dataSource, QObject* parent=nullptr);

	virtual void setViewTimeRange(const TimeRange& range) override;
	virtual ValueRange valueRange() const override;	//!< return value range for currenr view timerange
	virtual TimeRange timeRange() const override;	//!< return whole timerange

	DataSource dataSource() const;

private:
	QtCharts::QCandlestickSet* createSet(const Candle *candle) const;

	QPointer<QtCharts::QChart> mChart;
	DataSource mDataSource;
	QPointer<QtCharts::QCandlestickSeries> mCandleStickSeries;
	TimeRange mViewTimeRange;
private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);
signals:
	void candlesAdppended(int);	//!< в качестве аргумента - кол-во добавленых свечек
};
