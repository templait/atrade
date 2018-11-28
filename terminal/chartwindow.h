#pragma once

#include "chartwindowconf.h"

#include <QWidget>
#include <configuration.h>
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
	ChartWindow(const Configuration& configuration, QWidget* parent=nullptr);
	~ChartWindow();

	void loadConfiguration(const Configuration &configuration);
	const Configuration& configuration() const;
	ChartWindowConf &conf();
	static Configuration defaultConfiguration();

	void saveConfiguration(QSettings& settings) const;
	void loadConfiguration(QSettings& settings);

private:
	Ui::ChartWindow *ui;
	QGraphicsWidget* mGraphicsWidget;
	QGraphicsGridLayout* mSceneLayout;
	qreal mCandleWidth;
	QList<ChartWidget*> mChartWidgets;
	Configuration mConfiguration;
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
	ChartWidget* cregetChartWidget(const Configuration &configuration, int widgetNum=0);

	void onCandlesAppend(const DataSource&, int);


	// QWidget interface
protected:
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *event);
};
