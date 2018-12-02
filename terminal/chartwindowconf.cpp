#include "chartwindowconf.h"
#include "chartconf.h"

ChartWindowConf::ChartWindowConf() : AddChildParentConf<TimeIntervalConf, ChartConf>("ChartWindow")
{
	setTitle("Chart window");
	setTimeInterval(ETimeInterval::IntervalD1);
}
