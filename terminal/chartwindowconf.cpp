#include "chartwindowconf.h"

ChartWindowConf::ChartWindowConf(BConf *parent) : TimeIntervalConf(parent)
{

}

bool ChartWindowConf::canAppendChild(const BConf *child) const
{
	ChartConf chartConf;
	return child->metaObject()->className() == chartConf.metaObject()->className();
}
