#pragma once


#include <datasources/bdatasource.h>
#include <candle.h>

class QHostAddress;
class QTcpSocket;
class QFile;

class FileDataSource : public BDataSource
{
	Q_OBJECT
public:
	FileDataSource(const QString &fileName, QObject* parent=nullptr);
	virtual ~FileDataSource() override;
private:
	struct FDSSettings
	{
		QString fileName;
		QString className;
		QString code;
		ETimeInterval interval;
		bool operator==(const FDSSettings& other)const;
	} mSettings;

	QList<Candle> mCandles;
	QFile *mFile;

	QString sourceName() const;
	void readData();
	bool readSettings(const QJsonObject& obj);

	// BDataSource interface
public:
	virtual int size() const override;
	virtual void populate() override;
	virtual const Candle * at(int index) const override;
	virtual bool isActive() const override;
	virtual QString errorString() const override;
	virtual ETimeInterval interval() const override;
	virtual bool isSame(const SerialT &other) const override;
};
