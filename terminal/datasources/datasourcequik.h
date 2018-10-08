#pragma once

#include <QAbstractSocket>
#include <QTextStream>

#include <datasources/bdatasource.h>
#include <candle.h>

class QHostAddress;
class QTcpSocket;

class DataSourceQUIK : public BDataSource
{
	Q_OBJECT
public:
	DataSourceQUIK(ETimeInterval interval, const QString &className, const QString &code, const QString &hostName, quint16 port, QObject* parent=0);
	~DataSourceQUIK();
private:
	void connectToTerminal();
	void onIntervalChanged() override;

	QString sourceName() const;
	QTcpSocket *mSocket;
	QTextStream mStream;
	QList<Candle> mCandles;

	struct
	{
		QString hostName;
		quint16 port;
		QString className;
		QString code;
	} mSettings;

private slots:
	void onStateChanged(QAbstractSocket::SocketState socketState);
	void onReadyRead();
	void onError(QAbstractSocket::SocketError);

	// IDataSource interface
public:
	int size() const override;
	const Candle &operator [](int index) const override;
	bool isActive() const override;
	QString errorString() const override;
};
