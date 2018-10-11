#pragma once

#include <QCandlestickSeries>
#include "series/bseriesex.h"

class BDataSource;
class Candle;

class CandlestickSeries : public QtCharts::QCandlestickSeries, public BSeriesEx
{
	Q_OBJECT
public:
	CandlestickSeries(BDataSource* dataSource, QObject* parent=0);

private slots:
	void onCandlesAppended(int count);
	void onCandleUpdated(int index);

	// BSeriesEx interface
public:
	ValueRange valueRange(const QDateTime& start, const QDateTime& end) const override;
	TimeRange timeRange() const override;
	void setViewTimeRange(const TimeRange &range) override;
private:
	TimeRange mTimeRange;
	QtCharts::QCandlestickSet* createSet(const Candle& candle) const;
	bool isCandleInTimeRange(const Candle& candle) const;
};
