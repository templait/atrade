#include "datasourcequik.h"

#include <log.h>

#include <QTcpSocket>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

DataSourceQUIK::DataSourceQUIK(ETimeInterval interval, const QString &className, const QString &code, const QString &hostName, quint16 port, QObject* parent) :
	BDataSource(parent)
{
	mSettings.interval = interval;
	mSettings.className = className;
	mSettings.code = code;
	mSettings.hostName = hostName;
	mSettings.port = port;

	mSocket = new QTcpSocket(this);
	mStream.setDevice(mSocket);
	connect(mSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(onStateChanged(QAbstractSocket::SocketState)));
	connect(mSocket, SIGNAL(readyRead()), SLOT(onReadyRead()));
	connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onError(QAbstractSocket::SocketError)));
	connectToTerminal();
}

DataSourceQUIK::~DataSourceQUIK()
{
	if(isActive()) mSocket->close();
}

void DataSourceQUIK::connectToTerminal()
{
	mSocket->connectToHost(mSettings.hostName, mSettings.port);
}

int DataSourceQUIK::size() const
{
	return mCandles.size();
}

const Candle &DataSourceQUIK::operator[](int index) const
{
	return mCandles.at(index);
}

bool DataSourceQUIK::isActive() const
{
	return mSocket->state() == QAbstractSocket::ConnectedState;
}

QString DataSourceQUIK::errorString() const
{
	return mSocket->errorString();
}

QString DataSourceQUIK::sourceName() const
{
	return QString("QUIK DS: %1:%2, %3:%4").arg(mSettings.hostName).arg(mSettings.port).arg(mSettings.className).arg(mSettings.code);
}
// telnet command
// (echo '{"command":"GetDataSource", "Class":"TQBR", "Code":"SBER", "TimeFrame":1440}'; sleep 1) | telnet 192.168.9.63 5000
void DataSourceQUIK::onStateChanged(QAbstractSocket::SocketState socketState)
{
	switch(socketState)
	{
	case QAbstractSocket::ConnectedState:
	{
		QString cmd = QString("{\"command\":\"GetDataSource\", \"Class\":\"%1\", \"Code\":\"%2\", \"TimeFrame\":%3}\n").arg("TQBR").arg("SBER").arg(60*24);
		mStream << cmd;
		mStream.flush();
	}
		break;
	case QAbstractSocket::UnconnectedState:
		QTimer::singleShot(5000, this, &DataSourceQUIK::connectToTerminal);
		break;
	default:
		break;
	}
}

void DataSourceQUIK::onReadyRead()
{
	QString line;
	int candlesApp = 0;
	while(mSocket->canReadLine())
	{
		line = mStream.readLine();
		QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
		if(doc.isObject())
		{
			QJsonObject obj = doc.object();
			QDateTime time = QDateTime::fromString(obj.value("T").toString(), "yyyy-MM-dd hh:mm:ss:zzz");
			int index = obj.value("I").toInt();
			if(time.isValid() && index > 0)
			{
				index--;
				Candle c(
							obj.value("H").toDouble(),
							obj.value("L").toDouble(),
							obj.value("O").toDouble(),
							obj.value("C").toDouble(),
							obj.value("V").toDouble(),
							time
						);
				if(mCandles.size()==index) // Добавим свечку в конец
				{
					mCandles << c;
					candlesApp++;
				}
				else if(mCandles.size() > index)	// Обнови свечку
				{	mCandles[index] = c;	}
			}
		}
	}
	if(candlesApp>0)
	{	emit candlesAppended(candlesApp);	}
}

void DataSourceQUIK::onError(QAbstractSocket::SocketError)
{
	Log::warning(sourceName() + ": \"" + errorString() + '"');
}

ETimeInterval DataSourceQUIK::interval() const
{
	return mSettings.interval;
}
