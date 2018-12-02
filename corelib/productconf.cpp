#include "productconf.h"

#include <QVariant>

ProductConf::ProductConf(const QString &name) : BConf(name)
{

}

ProductID ProductConf::productID() const
{
	return mProductID;
}

void ProductConf::setProductID(ProductID productId)
{
	mProductID = productId;
}

void ProductConf::setParam(const QString &name, const QVariant &value)
{
	mParams[name] = value;
}

QVariant ProductConf::param(const QString &name) const
{
	QVariant rv;
	if(mParams.contains(name))
	{	rv = mParams["name"];	}
	return rv;
}

bool ProductConf::isSame(const BConf &other) const
{
	bool rv = false;
	if(const ProductConf* productConf = dynamic_cast<const ProductConf*>(&other))
	{
		rv = BConf::isSame(other)
		        && mProductID==productConf->mProductID
		        && mParams==productConf->mParams;
	}
	return rv;
}
