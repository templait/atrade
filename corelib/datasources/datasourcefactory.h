#pragma once

#include <datasources/bdatasource.h>
#include <datasources/datasourceconf.h>
#include <factory.hpp>

typedef Factory<BDataSource, DataSourceConf> DataSourceFactory;
