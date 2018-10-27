#pragma once

#include <QWidget>
#include <types.h>
#include <datasources/datasource.h>

class ChartWidget;
class QVBoxLayout;
class QGraphicsView;
class QGraphicsWidget;
class QGraphicsGridLayout;
class QScrollBar;

namespace QtCharts
{
class QAbstractSeries;
}

class ChartWindow : public QWidget
{
	Q_OBJECT
public:
	ChartWindow(QWidget* parent=nullptr);

	ChartWidget *addDataSource(DataSource dataSource, int widgetNum=0);

private:
	QVBoxLayout *mLayout;
	QGraphicsView *mGraphicsView;
	QGraphicsWidget* mGraphicsWidget;
	QGraphicsGridLayout* mSceneLayout;
	QScrollBar* mScrollBar;
	ETimeInterval mTimeInterval;
	qreal mCandleWidth;

	QList<ChartWidget*> mChartWidgets;

	void adjustScroll();
	void setScrollValue(int value);
	TimeRange seriesTimeRange() const;
	qint64 timeFrame() const;	//!< Размер временного окна, помещаемого в график.
	void setViewTimeRange(const TimeRange &range);
	void adjustGraphicsScene();
	int rescaleInt64(qint64 value) const;

private slots:
	void onCandlesAppend(DataSource, int);

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
};
