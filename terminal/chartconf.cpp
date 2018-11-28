#include "chartconf.h"

ChartConf::ChartConf(const QString &name) : BConf(name.isEmpty() ? "Chart" : name)
{
	setTitle("Chart");
}
