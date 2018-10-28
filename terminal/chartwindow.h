#pragma once

#include <QWidget>
#include <types.h>
#include <datasources/datasource.h>

class ChartWidget;
class QGraphicsWidget;
class QGraphicsGridLayout;
class QScrollBar;

namespace QtCharts
{
class QAbstractSeries;
}

namespace Ui
{
class ChartWindow;
}

class ChartWindow : public QWidget
{
	Q_OBJECT
public:
	ChartWindow(QWidget* parent=nullptr);
	~ChartWindow();

	ChartWidget *addDataSource(DataSource dataSource, int widgetNum=0);

private:
	Ui::ChartWindow *ui;
	QGraphicsWidget* mGraphicsWidget;
	QGraphicsGridLayout* mSceneLayout;
	ETimeInterval mTimeInterval;
	qreal mCandleWidth;

	QList<ChartWidget*> mChartWidgets;

	void adjustScroll();
	void setScrollValue(int value);
	TimeRange seriesTimeRange() const;
	qint64 timeFrame() const;	//!< Размер времянного окна, помещаемого в график.
	void setViewTimeRange(const TimeRange &range);
	void adjustGraphicsScene();
	void setTimeInterval(ETimeInterval interval);
	int rescaleInt64(qint64 value) const;

private slots:
	void onCandlesAppend(DataSource, int);

	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
};
