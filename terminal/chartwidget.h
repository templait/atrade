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
	ChartWidget(ETimeInterval interval, const Configuration& configuration, QGraphicsItem* parent=nullptr);
	void addDataSource(const Configuration& dataSource);
	void addIndicator(BIndicator* indicator);
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
	void candlesAppended(DataSource dataSource, int count);
};
