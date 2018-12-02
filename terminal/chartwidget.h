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
class ChartConf;

class ChartWidget : public QGraphicsWidget
{
	Q_OBJECT
public:
	ChartWidget(QGraphicsItem* parent=nullptr);
	ChartWidget(const ChartConf &chartConf, QGraphicsItem* parent=nullptr);
	void addDataSource(const DataSourceConf &dataSource);
	void addIndicator(const IndicatorConf &indicator);
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

private slots:
	void onCandlesAppended(int count);
	void onTimeRangeChanged(QDateTime min, QDateTime max);
signals:
	void candlesAppended(const DataSource& dataSource, int count);
};
