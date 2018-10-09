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
	QDateTime timeFrame() const;	//!< Размер временного окна, помещаемого в график.
	qreal secsInInterval() const;
	void setViewTimeRange(TimeRange range);
	TimeRange seriesTimeRange() const;
	void adjustScroll();
	void adjustValueAxises();
	void setScrollValue(int value);

	BDataSource *mDataSource;
	QScrollBar* mScrollBar;
	QVBoxLayout *mLayout;
	ETimeInterval mTimeInterval;
	qreal mCandleWidth;

	QList<ChartWidget*> mChartWidgets;
private slots:

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
};
