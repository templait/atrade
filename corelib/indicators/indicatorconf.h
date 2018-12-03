#pragma once

#include <productconf.h>

class IndicatorConf final : public ProductConf
{
public:
	IndicatorConf();

	// BConf interface
public:
	virtual bool isSame(const BConf &) const override;
};
