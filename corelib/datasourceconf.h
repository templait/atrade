#pragma once

#include <productconf.h>

class DataSourceConf : public ProductConf
{
	DataSourceConf() = delete;
public:
	DataSourceConf(const QString& name) : ProductConf(name){}
	virtual ~DataSourceConf() override{}
};
