#pragma once

#include "chartconf.h"

#include <timeintervalconf.h>
#include <addchildparentconf.hpp>

class ChartWindowConf final : public AddChildParentConf<TimeIntervalConf, ChartConf>
{
public:
	ChartWindowConf();
};
