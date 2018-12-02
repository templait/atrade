#pragma once

#include <indicators/bindicator.h>
#include <indicators/indicatorconf.h>
#include <factory.hpp>

typedef Factory<BIndicator, IndicatorConf> IndicatorFactory;
