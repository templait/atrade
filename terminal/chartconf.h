#pragma once

#include <bconf.h>

class ChartConf : public BConf
{
	Q_OBJECT
public:
	ChartConf(BConf* parent=nullptr);
};
