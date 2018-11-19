#include "lualineindicatorfactory.h"
#include "lualineindicator.h"
#include "lualineindicatorconfigurationeditor.h"

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
	
	return new LuaLineIndicator(luaPath +'/'+ fileName, DataSourceFactory::instance().product(configuration["DataSource"]));
}

Configuration LuaLineIndicatorFactory::defaultConfiguration() const
{
	Configuration rv;

	QString name = productName();
	rv.setName(name);
	rv.setValue(mTypeMap[mType]);
	rv.setTitle(QObject::tr("Линейный индикатор Lua", name.toLocal8Bit()));
	rv.setUserEditableMap(Configuration::Title);
	return rv;
}

ConfigurationEditorModule *LuaLineIndicatorFactory::createConfigurationEditor(const QModelIndex &configuration, QWidget *parent) const
{
	return new LuaLineIndicatorConfigurationEditor(configuration, parent);
}
