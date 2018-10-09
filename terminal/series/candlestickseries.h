#pragma once

#include <QCandlestickSeries>
#include "series/bseriesex.h"

class BDataSource;

class CandlestickSeries : public QtCharts::QCandlestickSeries, public BSeriesEx
{
	Q_OBJECT
public:
	CandlestickSeries(BDataSource* dataSource, QObject* parent=0);

	ValueRange valueRange(const QDateTime& start, const QDateTime& end) const override;
	TimeRange timeRange() const override;
private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);
};
