#include "chartwindowconf.h"
#include "chartconf.h"

ChartWindowConf::ChartWindowConf(BConf *parent) : TimeIntervalConf(parent)
{
	appendChildType<ChartConf>();
	setTitle(tr("Chart window"));
	setTimeInterval(ETimeInterval::IntervalD1);
}
