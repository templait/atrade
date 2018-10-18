#pragma once

#include "bseries.h"
#include <QObject>

namespace QtCharts
{
class QChart;
class QLineSeries;
}

class BIndicator;

class IndicatorSeries : public QObject, public BSeries
{
	Q_OBJECT
public:
	IndicatorSeries(QtCharts::QChart *chart, BIndicator *indicator, QObject * parent=0);
	virtual ~IndicatorSeries(){}
private:
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
