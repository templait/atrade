#pragma once

#include <QAbstractSocket>
#include <QTextStream>

#include <datasources/bdatasource.h>
#include <candle.h>

class QHostAddress;
class QTcpSocket;

class QuikDataSource : public BDataSource
{
	Q_OBJECT
public:
	QuikDataSource(ETimeInterval interval, const QString &className, const QString &code, const QString &hostName, quint16 port, QObject* parent=nullptr);
	virtual ~QuikDataSource() override;
private:
	void connectToTerminal();
	int quikInterval() const;

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
		ETimeInterval interval;
	} mSettings;

private slots:
	void onStateChanged(QAbstractSocket::SocketState socketState);
	void onReadyRead();
	void onError(QAbstractSocket::SocketError);

	// IDataSource interface
public:
	virtual ETimeInterval interval() const override;
	virtual int size() const override;
	virtual const Candle * at(int index) const override;
	virtual bool isActive() const override;
	virtual QString errorString() const override;
};
