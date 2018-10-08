#pragma once

#include "datasources/bdatasource.h"

#include <QWidget>

class QScrollBar;
class ChartWidget;
class QVBoxLayout;
class BSeriesEx;

namespace QtCharts
{
class QAbstractSeries;
}

class ChartWindow : public QWidget
{
	Q_OBJECT
public:
	ChartWindow(QWidget* parent=0);
	~ChartWindow();

	void addSeries(BSeriesEx *series, int widgetNum=0);

private:
	BDataSource *mDataSource;
	QScrollBar* mScrollBar;
	QVBoxLayout *mLayout;

	QList<ChartWidget*> mChartWidgets;
};
