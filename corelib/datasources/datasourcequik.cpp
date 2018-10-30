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

int DataSourceQUIK::quikInterval() const
{
	int rv = -1;
	switch(interval())
	{
	case IntervalM1:
		rv = 1;
		break;
	case IntervalM5:
		rv = 5;
		break;
	case IntervalM10:
		rv = 10;
		break;
	case IntervalM15:
		rv = 15;
		break;
	case IntervalM20:
		rv = 20;
		break;
	case IntervalM30:
		rv = 30;
		break;
	case IntervalH1:
		rv = 60;
		break;
	case IntervalH2:
		rv = 60*2;
		break;
	case IntervalH4:
		rv = 60*4;
		break;
	case IntervalD1:
		rv = 60*24;
		break;
	case IntervalW1:
		rv = 60*24*7;
		break;
	case IntervalMN1:
		rv = 23200;
		break;
	default:
		rv = -1;
	}
	return rv;
}

int DataSourceQUIK::size() const
{
	return mCandles.size();
}

const Candle *DataSourceQUIK::at(int index) const
{
	const Candle * rv = 0;
	if(index < size())
	{	rv = &(mCandles.at(index));	}
	return rv;
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
// (echo '{"Command":"GetDataSource", "Continue":1, "Class":"TQBR", "Code":"SBER", "TimeFrame":1440}'; sleep 1) | telnet 192.168.9.63 5000
void DataSourceQUIK::onStateChanged(QAbstractSocket::SocketState socketState)
{
	switch(socketState)
	{
	case QAbstractSocket::ConnectedState:
	{
		QString cmd = QString("{\"Command\":\"GetDataSource\", \"Continue\":1, \"Class\":\"%1\", \"Code\":\"%2\", \"TimeFrame\":%3}\n").arg(mSettings.className).arg(mSettings.code).arg(quikInterval());
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
	int candles = mCandles.size();
	QSet<int> needUpdate;
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
							time,
							mSettings.interval
						);
				if(mCandles.size()==index) // Добавим свечку в конец
				{	mCandles << c;			}
				else if(mCandles.size() > index)	// Обнови свечку
				{
					mCandles[index] = c;
					needUpdate << index;
				}
			}
		}
	}
	if(candles<mCandles.size())
	{	emit candlesAppended(mCandles.size()-candles);	}
	for(int index : needUpdate) emit candleUpdated(index);
}

void DataSourceQUIK::onError(QAbstractSocket::SocketError)
{
	Log::warning(sourceName() + ": \"" + errorString() + '"');
}

ETimeInterval DataSourceQUIK::interval() const
{
	return mSettings.interval;
}
