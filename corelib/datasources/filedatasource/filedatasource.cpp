#include "filedatasource.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <tools.h>
#include "log.h"


FileDataSource::FileDataSource(const QString &fileName, QObject* parent) : BDataSource(parent), mFile(nullptr)
{
	mFile = new QFile(fileName, this);
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
							mInfo.interval
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
	mInfo.className = obj.value("ClassName").toString();
	mInfo.code = obj.value("Code").toString();
	if(strInterval.isNull() || mInfo.className.isNull() || mInfo.code.isNull())
	{
		Log::warning(sourceName() + ": \"" + tr("settings string isn't valid") + '"');
	}
	else
	{
		if(strInterval == "TICK")
			mInfo.interval = IntervalTICK;
		else if(strInterval == "M1")
			mInfo.interval = IntervalM1;
		else if(strInterval == "M5")
			mInfo.interval = IntervalM5;
		else if(strInterval == "M10")
			mInfo.interval = IntervalM10;
		else if(strInterval == "M15")
			mInfo.interval = IntervalM15;
		else if(strInterval == "M20")
			mInfo.interval = IntervalM20;
		else if(strInterval == "M30")
			mInfo.interval = IntervalM30;
		else if(strInterval == "H1")
			mInfo.interval = IntervalH1;
		else if(strInterval == "H2")
			mInfo.interval = IntervalH2;
		else if(strInterval == "H4")
			mInfo.interval = IntervalH4;
		else if(strInterval == "D1")
			mInfo.interval = IntervalD1;
		else if(strInterval == "W1")
			mInfo.interval = IntervalW1;
		else if(strInterval == "MN1")
			mInfo.interval = IntervalMN1;
		else
			mInfo.interval = IntervalUnknown;

		if(mInfo.interval == IntervalUnknown)
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

void FileDataSource::populate()
{
	if(mFile->open(QIODevice::ReadOnly))
	{	readData();	}
	else
	{	Log::error(QString("%1.%2:%3").arg(__CLASS_NAME__).arg(sourceName()).arg(errorString()));	}
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
	return mInfo.interval;
}

bool FileDataSource::isSame(const SerialT &other) const
{
	bool rv = false;
	if(const FileDataSource* fds = dynamic_cast<const FileDataSource*>(&other))
	{
		rv = mFile->fileName()==fds->mFile->fileName();
	}
	return rv;
}

SerialInfo FileDataSource::info() const
{
	return {__CLASS_NAME__, QString("I:%1, %2.%3").arg(intervalToString(mInfo.interval)).arg(mInfo.className).arg(mInfo.code)};
}
