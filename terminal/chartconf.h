#pragma once

#include <datasources/datasourceconf.h>
#include <parentconf.hpp>

class ChartConf final : public ParentConf<BConf, DataSourceConf>
{
public:
	ChartConf();
};
