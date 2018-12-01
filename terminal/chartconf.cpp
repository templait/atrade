#include "chartconf.h"

ChartConf::ChartConf() : ParentConf<BConf, DataSourceConf>("Chart")
{
	setTitle("Chart");
}

BConf *ChartConf::clone() const
{
	return new ChartConf(*this);
}
