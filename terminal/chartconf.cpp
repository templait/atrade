#include "chartconf.h"

ChartConf::ChartConf() : ParentConf<BConf, DataSourceConf>("Chart")
{
	setTitle("Chart");
}
