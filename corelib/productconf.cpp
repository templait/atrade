#include "productconf.h"

ProductConf::ProductConf(const QString &name) : BConf(name.isEmpty() ? "Product" : name)
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

bool ProductConf::isSame(const BConf &other) const
{
	bool rv = false;
	if(const ProductConf* productId = dynamic_cast<const ProductConf*>(&other))
	{
		rv = BConf::isSame(other) && mProductID==productId->mProductID;
	}
	return rv;
}
