#pragma once

#include <QGraphicsWidget>
#include <types.h>
#include <datasources/datasource.h>

namespace QtCharts
{
class QChart;
class QValueAxis;
class QDateTimeAxis;
}

class BSeries;
class BIndicator;
class Configuration;

class ChartWidget : public QGraphicsWidget
{
	Q_OBJECT
public:
	ChartWidget(ETimeInterval interval, QGraphicsItem* parent=nullptr);
	ChartWidget(ETimeInterval interval, const BConf &conf, QGraphicsItem* parent=nullptr);
	void addDataSource(const ProductConf &dataSource);
	void addIndicator(const ProductConf &indicator);
	TimeRange viewTimeRange() const;
	void setViewTimeRange(const TimeRange &range);
	TimeRange timeRange() const;	// return whole timerange
	QRectF plotArea() const;

	QtCharts::QValueAxis* valueAxis();
	QtCharts::QDateTimeAxis* timeAxis();

private:
	void adjustValueAxis();

	QtCharts::QChart *mChart;
	QList<BSeries*> mSeries;
	QtCharts::QValueAxis *mValueAxis;
	QtCharts::QDateTimeAxis *mTimeAxis;
	ETimeInterval mTimeInterval;

private slots:
	void onCandlesAppended(int count);
	void onTimeRangeChanged(QDateTime min, QDateTime max);
signals:
	void candlesAppended(const DataSource& dataSource, int count);
};
