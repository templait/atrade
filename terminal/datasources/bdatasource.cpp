#include "bdatasource.h"

BDataSource::BDataSource(QObject *parent) : QObject(parent)
{

}

const Candle &BDataSource::first() const
{
	return operator[](0);
}

const Candle &BDataSource::last() const
{
	return operator[](size()-1);
}

QList<Candle> BDataSource::getTimeRange(const TimeRange& range) const
{
	QList<Candle> rv;
	for(int i=0; i<size(); i++)
	{
		const Candle & candle = (*this)[i];
		if(candle.time() >= range.first && candle.time() <= range.second)
		{
			rv << candle;
		}
	}
	return rv;
}

