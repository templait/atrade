#pragma once

#include "chartconf.h"

#include <timeintervalconf.h>
#include <parentconf.hpp>

class ChartWindowConf final : public ParentConf<TimeIntervalConf, ChartConf>
{
public:
	Q_INVOKABLE ChartWindowConf(const QString& name=QString());
};
