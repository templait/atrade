#pragma once

#include <timeintervalconf.h>

class ChartWindowConf final : public TimeIntervalConf
{
	Q_OBJECT
public:
	ChartWindowConf(BConf* parent=nullptr);
};
