#include "chartwindowconf.h"
#include "chartconf.h"

ChartWindowConf::ChartWindowConf(const QString& name) : ParentConf<TimeIntervalConf, ChartConf>(name.isEmpty() ? "ChartWindow" : name)
{
	setTitle("Chart window");
	setTimeInterval(ETimeInterval::IntervalD1);
}

void ChartWindowConf::serialize(QDataStream &out) const
{
	TimeIntervalConf::serialize(out);
	out << children().count();
	for(const ChartConf& child : children())
	{	child.serialize(out);	}
}

void ChartWindowConf::deserialize(QDataStream &in)
{
	TimeIntervalConf::deserialize(in);
	int childrenCount;
	in >> childrenCount;
	for(int i=0; i<childrenCount; i++)
	{
		ChartConf chart;
		chart.deserialize(in);
		appendChild(chart);
	}
}
