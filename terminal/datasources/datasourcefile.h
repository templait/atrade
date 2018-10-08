#pragma once


#include "bdatasource.h"
#include <candle.h>

class QHostAddress;
class QTcpSocket;

class DataSourceFile : public BDataSource
{
	Q_OBJECT
public:
	DataSourceFile(ETimeInterval interval, const QString &className, const QString &code, QObject* parent=0);
	~DataSourceFile();
private:
	void onIntervalChanged() override;

	QList<Candle> mCandles;

	// BDataSource interface
public:
	int size() const override;
	const Candle &operator [](int index) const override;
	bool isActive() const override;
	QString errorString() const override;
};
