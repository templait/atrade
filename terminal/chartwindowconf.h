#pragma once

#include "chartconf.h"

#include <timeintervalconf.h>
#include <parentconf.hpp>

class ChartWindowConf final : public ParentConf<TimeIntervalConf, ChartConf>
{
public:
	Q_INVOKABLE ChartWindowConf(const QString& name=QString());

	// TimeIntervalConf interface
public:
	virtual void serialize(QDataStream &out) const override;
	virtual void deserialize(QDataStream &in) override;
};
