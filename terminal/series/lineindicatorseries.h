#pragma once

#include "bseries.h"
#include <QObject>

namespace QtCharts
{
class QChart;
class QLineSeries;
}

class BIndicator;
class Point;

class LineIndicatorSeries : public QObject, public BSeries
{
	Q_OBJECT
public:
	LineIndicatorSeries(QtCharts::QChart *chart, BIndicator *indicator, QObject * parent=0);
	virtual ~LineIndicatorSeries(){}
private:
	void appendPoints(const QList<const Point *> points);

	QtCharts::QChart *mChart;
	BIndicator* mIndicator;
	TimeRange mViewTimeRange;
	QList<QtCharts::QLineSeries*> mSeries;
private slots:
	void onPointsAppended(int count);
	void onPointUpdated(int index);

	// BSeries interface
public:
	virtual void setViewTimeRange(const TimeRange &range) override;
	virtual ValueRange valueRange() const override;
	virtual TimeRange timeRange() const override;
};
