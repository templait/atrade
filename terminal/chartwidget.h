#pragma once

#include <QGraphicsWidget>
#include "types.h"

namespace QtCharts
{
class QChart;
class QValueAxis;
class QDateTimeAxis;
}

class BSeriesEx;

class ChartWidget : public QGraphicsWidget
{
	Q_OBJECT
public:
	ChartWidget(QGraphicsItem* parent=0);
	void addSeries(BSeriesEx *series);
	TimeRange viewTimeRange() const;
	void setViewTimeRange(const TimeRange &range);
	TimeRange seriesTimeRange() const;
	QRectF plotArea() const;
	void adjustValueAxis();

	QtCharts::QValueAxis* valueAxis();
	QtCharts::QDateTimeAxis* timeAxis();

private:
	QtCharts::QChart *mChart;
	QtCharts::QValueAxis *mValueAxis;
	QtCharts::QDateTimeAxis *mTimeAxis;

private slots:
	void onCountChanged();
	void onTimeRangeChanged(QDateTime min, QDateTime max);
};
