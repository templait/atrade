#pragma once

#include <types.h>

class BSeries
{
public:
	virtual void setViewTimeRange(const TimeRange& range) = 0;
	virtual ValueRange valueRange() const = 0;	//!< return value range for currenr view timerange
	virtual TimeRange timeRange() const = 0;	//!< return whole timerange
	virtual void setName(const QString& name) {Q_UNUSED(name);}

	virtual ~BSeries(){}
};
