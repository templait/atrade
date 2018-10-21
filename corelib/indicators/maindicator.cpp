#include "maindicator.h"

MAIndicator::MAIndicator(DataSource dataSource, MAIndicator::TMAType type, TOtputType outputType, QObject *parent)
    : CandleAdapterIndicator(dataSource,outputType, parent)
    , mMAType(type)
{

}

Point MAIndicator::candle2point(const Candle &candle) const
{

}
