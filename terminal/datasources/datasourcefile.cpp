#include "datasourcefile.h"


DataSourceFile::DataSourceFile(ETimeInterval interval, const QString &className, const QString &code, QObject* parent) : BDataSource(interval, parent)
{

}

DataSourceFile::~DataSourceFile()
{

}


int DataSourceFile::size() const
{
	return mCandles.size();
}

const Candle &DataSourceFile::operator[](int index) const
{
	return mCandles.at(index);
}

bool DataSourceFile::isActive() const
{
	return false;
}

QString DataSourceFile::errorString() const
{
	return QString();
}

void DataSourceFile::onIntervalChanged()
{

}
