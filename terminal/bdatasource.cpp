#include "bdatasource.h"

BDataSource::BDataSource(ETimeInterval interval, QObject *parent) : QObject(parent), mInterval(interval)
{

}

void BDataSource::setInterval(ETimeInterval interval)
{
	mInterval = interval;
	onIntervalChanged();
}
