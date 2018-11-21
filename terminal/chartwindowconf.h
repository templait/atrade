#pragma once
#include "chartconf.h"

#include <timeintervalconf.h>

class ChartWindowConf final : public TimeIntervalConf
{
	Q_OBJECT
public:
	ChartWindowConf(BConf* parent=nullptr);
	void append() const{}
	// BConf interface
public:
	bool canAppendChild(const BConf *child) const override;
};
