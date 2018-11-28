#include "chartwindowconf.h"
#include "chartconf.h"

ChartWindowConf::ChartWindowConf() : TimeIntervalConf()
{
	appendChildType<ChartConf>();
	setTitle(tr("Chart window"));
	setTimeInterval(ETimeInterval::IntervalD1);
}

void ChartWindowConf::serialize(QDataStream &out) const
{
	TimeIntervalConf::serialize(out);
}
