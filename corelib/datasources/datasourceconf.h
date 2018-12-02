#pragma once

#include <indicators/indicatorconf.h>
#include <parentconf.hpp>

class DataSourceConf final : public ParentConf<ProductConf, IndicatorConf>
{
public:
	DataSourceConf() : ParentConf<ProductConf, IndicatorConf>("DataSource"){}
};
