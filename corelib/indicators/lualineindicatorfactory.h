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
	virtual BIndicator *create(const Configuration &configuration) const override;
	virtual Configuration defaultConfiguration() const override;
	virtual ProductConfigurationEditor *createConfigurationEditor(const Configuration& configuration, QWidget* parent=nullptr) const override;
};
