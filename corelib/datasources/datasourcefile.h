#pragma once


#include <datasources/bdatasource.h>
#include <candle.h>

class QHostAddress;
class QTcpSocket;
class QFile;

class DataSourceFile : public BDataSource
{
	Q_OBJECT
public:
	DataSourceFile(const QString &fileName, QObject* parent=nullptr);
	virtual ~DataSourceFile() override;
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
	virtual int size() const override;
	virtual const Candle * at(int index) const override;
	virtual bool isActive() const override;
	virtual QString errorString() const override;
	virtual ETimeInterval interval() const override;
};
