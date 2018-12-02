#pragma once

#include "chartwindowconf.h"

#include <QWidget>
#include <types.h>
#include <datasources/datasource.h>

class ChartWidget;
class QGraphicsWidget;
class QGraphicsGridLayout;
class QSettings;

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
	ChartWindow(const ChartWindowConf &conf, QWidget* parent=nullptr);
	~ChartWindow();

	void loadConf(const ChartWindowConf &conf);
	ChartWindowConf &conf();
	static ChartWindowConf defaultConf();

	void saveConf(QSettings& settings) const;
	void loadConf(QSettings& settings);

private:
	Ui::ChartWindow *ui;
	QGraphicsWidget* mGraphicsWidget;
	QGraphicsGridLayout* mSceneLayout;
	qreal mCandleWidth;
	QList<ChartWidget*> mChartWidgets;
	ChartWindowConf mConf;

	void adjustScroll();
	void setScrollValue(int value);
	TimeRange seriesTimeRange() const;
	qint64 timeFrame() const;	//!< Размер времянного окна, помещаемого в график.
	void setViewTimeRange(const TimeRange &range);
	void adjustGraphicsScene();
	void setTimeInterval(ETimeInterval interval);
	ETimeInterval timeInterval() const;
	int rescaleInt64(qint64 value) const;
	void clear();
	ChartWidget* cregetChartWidget(const ChartConf &conf, int widgetNum=0);

	void onCandlesAppend(const DataSource&, int);


	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
};
