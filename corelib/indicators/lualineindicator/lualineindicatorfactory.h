#pragma once

#include <indicators/indicatorfactory.h>

class LuaLineIndicatorFactory : public IndicatorFactory::Unit
{
public:
	enum EIndicatorType {IndicatorMA};
	LuaLineIndicatorFactory(EIndicatorType type);
	virtual ~LuaLineIndicatorFactory() override{}
private:
	EIndicatorType mType;
	static QMap<EIndicatorType, QUuid> mTypeMap;
	
	// Unit interface
public:
	virtual BIndicator *create(const IndicatorConf &conf) const override;
	virtual IndicatorConf* createDefaultConf() const override;
};
