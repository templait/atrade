#include "maindicator.h"

MAIndicator::MAIndicator(const BDataSource *dataSource, MAIndicator::TMAType type, CandleAdapterIndicator::TOtputType outputType, QObject *parent)
    : BLineIndicator(dataSource, parent)
    , mMAType(type)
{

}
