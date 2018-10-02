#pragma once

#include <QWidget>

namespace QtCharts
{
class QChartView;
class QChart;
class QAbstractSeries;
}

class ChartWidget : public QWidget
{
	Q_OBJECT
public:
	ChartWidget(QWidget* parent=0);
	void addSeries(QtCharts::QAbstractSeries *series);
private:
	QtCharts::QChartView *mChartView;
	QtCharts::QChart *mChart;
private slots:
	void onCountChanged();
};
