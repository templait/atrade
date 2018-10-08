#pragma once

#include <QWidget>

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
private:
	QtCharts::QChartView *mChartView;
	QtCharts::QChart *mChart;
	QtCharts::QValueAxis *mValueAxis;
	QtCharts::QDateTimeAxis *mTimeAxis;
private slots:
	void onCountChanged();
};
