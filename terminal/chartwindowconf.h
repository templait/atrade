#pragma once

#include <timeintervalconf.h>

class ChartWindowConf final : public TimeIntervalConf
{
	Q_OBJECT
public:
	Q_INVOKABLE ChartWindowConf();

	// BConf interface
protected:
	virtual void serialize(QDataStream &out) const override;
	//virtual void deserialize(QDataStream &in) override;
};
