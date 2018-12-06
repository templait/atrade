#pragma once

#include <productconfhelper.hpp>
#include <datasources/datasourceconf.h>

class TimeIntervalConf;

class DataSourceConfHelper : public ProductConfHelper<DataSourceConf>
{
public:
	DataSourceConfHelper(ProductID id, DataSourceConf* conf=nullptr);
	virtual ~DataSourceConfHelper() override {}

	const TimeIntervalConf* timeIntervalConf() const;
};
