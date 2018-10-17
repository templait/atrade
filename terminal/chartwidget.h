#pragma once

#include <QGraphicsWidget>
#include "types.h"

namespace QtCharts
{
class QChart;
class QValueAxis;
class QDateTimeAxis;
}

class BSeriesEx; //kill
class DataSourceSeries;
class BDataSource;

class ChartWidget : public QGraphicsWidget
{
	Q_OBJECT
public:
	ChartWidget(QGraphicsItem* parent=0);
	void addDataSource(BDataSource* dataSource);
	TimeRange viewTimeRange() const;
	void setViewTimeRange(const TimeRange &range);
	TimeRange timeRange() const;	// return whole timerange
	QRectF plotArea() const;

	QtCharts::QValueAxis* valueAxis();
	QtCharts::QDateTimeAxis* timeAxis();

private:
	void adjustValueAxis();

	QtCharts::QChart *mChart;
	QList<DataSourceSeries*> mSeries;
	QtCharts::QValueAxis *mValueAxis;
	QtCharts::QDateTimeAxis *mTimeAxis;

private slots:
	void onCandlesAppended(int count);
	void onTimeRangeChanged(QDateTime min, QDateTime max);
signals:
	void candlesAppended(const BDataSource* dataSource, int count);
};
