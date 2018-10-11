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

