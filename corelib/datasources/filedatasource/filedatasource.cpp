#include "filedatasource.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <tools.h>
#include "log.h"


FileDataSource::FileDataSource(const QString &fileName, QObject* parent) : BDataSource(parent)
{
	mFile = new QFile(fileName, this);
	if(mFile->open(QIODevice::ReadOnly))
	{	readData();	}
	else
	{	Log::error(QString("%1.%2:%3").arg(__CLASS_NAME__).arg(sourceName()).arg(errorString()));	}
}

FileDataSource::~FileDataSource()
{

}

QString FileDataSource::sourceName() const
{
	return QString("File(%1)").arg(mFile->fileName());
}

void FileDataSource::readData()
{
	QTextStream stream(mFile);
	int lineNum = 0;
	mCandles.clear();
	while(! stream.atEnd())
	{
		QString line = stream.readLine();
		QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
		if(doc.isObject())
		{
			QJsonObject obj = doc.object();
			if(lineNum==0)
			{
				if(! readSettings(obj))
				{	break;	}
			}
			else
			{
				QDateTime time = QDateTime::fromString(obj.value("T").toString(), "yyyy-MM-dd hh:mm:ss:zzz");
				mCandles << Candle (
							obj.value("H").toDouble(),
							obj.value("L").toDouble(),
							obj.value("O").toDouble(),
							obj.value("C").toDouble(),
							obj.value("V").toDouble(),
							time,
							mSettings.interval
						);
			}

		}
		lineNum++;
	}
	if(mCandles.size() > 0)
	{	emit candlesAppended(mCandles.size()); }
}

bool FileDataSource::readSettings(const QJsonObject &obj)
{
	bool rv = false;
	QString strInterval = obj.value("Interval").toString();
	QString className = obj.value("ClassName").toString();
	QString code = obj.value("Code").toString();
	if(strInterval.isNull() || className.isNull() || code.isNull())
	{
		Log::warning(sourceName() + ": \"" + tr("settings string isn't valid") + '"');
	}
	else
	{
		if(strInterval == "TICK")
			mSettings.interval = IntervalTICK;
		else if(strInterval == "M1")
			mSettings.interval = IntervalM1;
		else if(strInterval == "M5")
			mSettings.interval = IntervalM5;
		else if(strInterval == "M10")
			mSettings.interval = IntervalM10;
		else if(strInterval == "M15")
			mSettings.interval = IntervalM15;
		else if(strInterval == "M20")
			mSettings.interval = IntervalM20;
		else if(strInterval == "M30")
			mSettings.interval = IntervalM30;
		else if(strInterval == "H1")
			mSettings.interval = IntervalH1;
		else if(strInterval == "H2")
			mSettings.interval = IntervalH2;
		else if(strInterval == "H4")
			mSettings.interval = IntervalH4;
		else if(strInterval == "D1")
			mSettings.interval = IntervalD1;
		else if(strInterval == "W1")
			mSettings.interval = IntervalW1;
		else if(strInterval == "MN1")
			mSettings.interval = IntervalMN1;
		else
			mSettings.interval = IntervalUnknown;

		if(mSettings.interval == IntervalUnknown)
		{	Log::warning(sourceName() + ": \"" + tr("Unknown time interval") + '"');	}
		else
		{	rv = true;	}
	}
	return rv;
}


int FileDataSource::size() const
{
	return mCandles.size();
}

const Candle *FileDataSource::at(int index) const
{
	const Candle * rv = nullptr;
	if(index < size())
	{	rv = &(mCandles.at(index));	}
	return rv;
}

bool FileDataSource::isActive() const
{
	return false;
}

QString FileDataSource::errorString() const
{
	return mFile->errorString();
}

ETimeInterval FileDataSource::interval() const
{
	return mSettings.interval;
}
