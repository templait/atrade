#include "lualineindicatorfactory.h"

#include "lualineindicator.h"
#include "lualineindicatorconfigurationeditor.h"

#include <QSettings>

#include <indicators/indicatorconf.h>
#include <datasources/datasourceconf.h>

QMap<LuaLineIndicatorFactory::EIndicatorType, QUuid> LuaLineIndicatorFactory::mTypeMap = {
    {	LuaLineIndicatorFactory::IndicatorMA, QUuid("53826e87-f97c-42d3-a043-0d041674ab5a")	}
};

LuaLineIndicatorFactory::LuaLineIndicatorFactory(LuaLineIndicatorFactory::EIndicatorType type)
	: IndicatorFactory::Unit(QString("Lua line indicator"), mTypeMap[type])
	, mType(type)
{
	
}

BIndicator *LuaLineIndicatorFactory::create(const IndicatorConf &conf) const
{
	QString fileName;
	switch(mType)
	{
	case IndicatorMA:
		fileName = "indicators/MA.lua";
		break;
	}
	
	QSettings settings;
	QString luaPath = settings.value("LuaDir").toString();
	
	LuaLineIndicator* rv=nullptr;

	if(/*const LuaLineIndicatorConf* llConf = */dynamic_cast<const IndicatorConf*>(&conf))
	{
		if(const DataSourceConf* dsConf = conf.findParent<DataSourceConf>())
		{
			rv = new LuaLineIndicator(luaPath +'/'+ fileName, DataSourceFactory::instance().product(*dsConf));
		}
		else
		{	Log::error(QString("%1.Configuration haven't DataSourceConf parents").arg(__CLASS_NAME__));	}
	}
	else
	{	Log::error(QString("%1.Configuration isn't IndicatorConf").arg(__CLASS_NAME__));	}

	return rv;
}

IndicatorConf *LuaLineIndicatorFactory::createDefaultConf() const
{
	IndicatorConf* rv = new IndicatorConf;
	rv->setProductID(mTypeMap[mType]);
	rv->setTitle(QObject::tr("Индикатор Lua", __CLASS_NAME__.toLocal8Bit()));
	return rv;
}
