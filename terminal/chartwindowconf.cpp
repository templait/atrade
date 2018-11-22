#include "chartwindowconf.h"
#include "chartconf.h"

ChartWindowConf::ChartWindowConf(BConf *parent) : TimeIntervalConf(parent)
{
	appendChildctor([](BConf* parentConf){return new ChartConf(parentConf);});
	setTitle(tr("Chart window"));
	setTimeInterval(ETimeInterval::IntervalD1);
}
