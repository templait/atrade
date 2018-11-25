#pragma once

#include <bconf.h>

class ChartConf : public BConf
{
	Q_OBJECT
	CONF_NAME("Chart")
public:
	Q_INVOKABLE ChartConf(BConf* parent=nullptr);
};
