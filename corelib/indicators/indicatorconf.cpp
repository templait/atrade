#include "indicatorconf.h"

IndicatorConf::IndicatorConf() : ProductConf("Indicator")
{

}

bool IndicatorConf::isSame(const BConf &/*other*/) const
{
	/*bool rv = ProductConf::isSame(other);
	if(rv)
	{
		if(const IndicatorConf* otherIndicator = dynamic_cast<const IndicatorConf*>(&other))
		{
			rv = parentConf()->isSame(*otherIndicator->parentConf());
		}
	}*/
	// покуда все индикаторы будут заново создаваться. т.к. не понятно каким образом сравнивать источник по
	// которому строится индикатор. Дело в том, что конфигурация индикатора, которая хранится в хэше фабрики,
	// содержит копию, в которой отсутствует родителькая конфигурация, тоесть конфигурация источника.
	return false;
}
