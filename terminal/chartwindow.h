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

	void addDataSource(BDataSource *dataSource, int widgetNum=0);

private:
	qint64 timeFrame() const;	//!< Размер временного окна, помещаемого в график.
	qint64 secsInInterval() const;
	void setViewTimeRange(TimeRange range);
	TimeRange seriesTimeRange() const;
	void adjustScroll();
	void adjustValueAxises();
	void setScrollValue(int value);

	QList<BDataSource*> mDataSources;
	QScrollBar* mScrollBar;
	QVBoxLayout *mLayout;
	ETimeInterval mTimeInterval;
	qreal mCandleWidth;

	QList<ChartWidget*> mChartWidgets;
private slots:
	void onCandlesAppend(int);

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
};
