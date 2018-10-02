#pragma once

#include <QCandlestickSeries>

class BDataSource;

class CandlestickSeries : public QtCharts::QCandlestickSeries
{
	Q_OBJECT
public:
	CandlestickSeries(QObject* parent=0);
	void setDataSource(BDataSource* dataSource);
private:
	BDataSource* mDataSource;
private slots:
	void onCandlesAppended(int count);
};
