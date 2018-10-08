#pragma once

#include <QCandlestickSeries>
#include "series/bseriesex.h"

class BDataSource;

class CandlestickSeries : public QtCharts::QCandlestickSeries, public BSeriesEx
{
	Q_OBJECT
public:
	CandlestickSeries(QObject* parent=0);
	void setDataSource(BDataSource* dataSource);

	QPair<qreal, qreal> valueRange(const QDateTime& start, const QDateTime& end) const override;
	QPair<QDateTime, QDateTime> timeRange() const override;
private:
	BDataSource* mDataSource;
private slots:
	void onCandlesAppended(int count);
};
