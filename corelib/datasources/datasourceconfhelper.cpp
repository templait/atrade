#include "datasourceconfhelper.h"

#include <timeintervalconf.h>

DataSourceConfHelper::DataSourceConfHelper(ProductID id, DataSourceConf *conf) : ProductConfHelper<DataSourceConf>(id, conf)
{

}

const TimeIntervalConf *DataSourceConfHelper::timeIntervalConf() const
{
	return conf().findParent<TimeIntervalConf>();
}
