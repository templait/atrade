#pragma once

#include <datasourceconf.h>
#include <parentconf.hpp>

class ChartConf final : public ParentConf<BConf, DataSourceConf>
{
public:
	ChartConf();

	// BConf interface
public:
	virtual BConf *clone() const override;
};
