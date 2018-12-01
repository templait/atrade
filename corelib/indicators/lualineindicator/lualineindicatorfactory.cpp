#include "lualineindicatorfactory.h"
/*
#include "lualineindicator.h"
#include "lualineindicatorconfigurationeditor.h"
#include "lualineindicatorconfnames.h"

#include <QSettings>

QMap<LuaLineIndicatorFactory::EIndicatorType, QUuid> LuaLineIndicatorFactory::mTypeMap = {
    {	LuaLineIndicatorFactory::IndicatorMA, QUuid("53826e87-f97c-42d3-a043-0d041674ab5a")	}
};

LuaLineIndicatorFactory::LuaLineIndicatorFactory(LuaLineIndicatorFactory::EIndicatorType type)
	: IndicatorFactory::Unit(QString("Lua line indicator"), mTypeMap[type])
	, mType(type)
{
	
}

BIndicator *LuaLineIndicatorFactory::create(const Configuration &configuration) const
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

	if(configuration[CN_SOURCE].childrenCount()==1)
	{
		const Configuration* dsConf = configuration[CN_SOURCE].childAt(0);
		rv = new LuaLineIndicator(luaPath +'/'+ fileName, DataSourceFactory::instance().product(*dsConf));
	}
	else
	{	Log::error(QString("%1.Source product doesn't defined.").arg(__CLASS_NAME__).arg(configuration.value().toUuid().toString()));	}

	return rv;
}

Configuration LuaLineIndicatorFactory::defaultConfiguration() const
{
	Configuration rv;

	QString name = productName();
	rv.setName(name);
	rv.setValue(mTypeMap[mType]);
	rv.setTitle(QObject::tr("Lua line indicator", name.toLocal8Bit()));
	rv.setUserEditableMap(Configuration::Title);
	rv.insertChild({CN_SOURCE});
	return rv;
}

ConfigurationEditorModule *LuaLineIndicatorFactory::createConfigurationEditor(const QModelIndex &configuration, QWidget *parent) const
{
	return new LuaLineIndicatorConfigurationEditor(configuration, parent);
}
*/
