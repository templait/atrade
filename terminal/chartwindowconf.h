#pragma once

#include <timeintervalconf.h>

class ChartWindowConf final : public TimeIntervalConf
{
	Q_OBJECT
public:
	Q_INVOKABLE ChartWindowConf(BConf* parent=nullptr);
};
