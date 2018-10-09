#pragma once


#include "bdatasource.h"
#include <candle.h>

class QHostAddress;
class QTcpSocket;
class QFile;

class DataSourceFile : public BDataSource
{
	Q_OBJECT
public:
	DataSourceFile(const QString &fileName, QObject* parent=0);
	virtual ~DataSourceFile();
private:
	struct
	{
		QString fileName;
		QString className;
		QString code;
		ETimeInterval interval;
	} mSettings;

	QList<Candle> mCandles;
	QFile *mFile;

	QString sourceName() const;
	void readData();
	bool readSettings(const QJsonObject& obj);

	// BDataSource interface
public:
	int size() const override;
	const Candle &operator [](int index) const override;
	bool isActive() const override;
	QString errorString() const override;
	ETimeInterval interval() const override;
};
