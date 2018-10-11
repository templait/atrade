#pragma once

#include <QWidget>
#include "types.h"

namespace QtCharts
{
class QChartView;
class QChart;
class QValueAxis;
class QDateTimeAxis;
}

class BSeriesEx;

class ChartWidget : public QWidget
{
	Q_OBJECT
public:
	ChartWidget(QWidget* parent=0);
	void addSeries(BSeriesEx *series);
	TimeRange viewTimeRange() const;
	void setViewTimeRange(const TimeRange& range);
	TimeRange seriesTimeRange() const;
	QRectF plotArea() const;
	void adjustValueAxis();

	QtCharts::QValueAxis* valueAxis();
	QtCharts::QDateTimeAxis* timeAxis();
private:
	QtCharts::QChartView *mChartView;
	QtCharts::QChart *mChart;
	QtCharts::QValueAxis *mValueAxis;
	QtCharts::QDateTimeAxis *mTimeAxis;
private slots:
	void onCountChanged();
	void onTimeRangeChanged(QDateTime min, QDateTime max);
signals:

};
