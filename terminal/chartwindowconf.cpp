#include "chartwindowconf.h"
#include "chartconf.h"

ChartWindowConf::ChartWindowConf(const QString& name) : ParentConf<TimeIntervalConf, ChartConf>(name.isEmpty() ? "ChartWindow" : name)
{
	setTitle("Chart window");
	setTimeInterval(ETimeInterval::IntervalD1);
}
