#include "productconf.h"

#include <QDataStream>
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
	{	rv = mParams[name];	}
	return rv;
}

void ProductConf::serialize(QDataStream &out) const
{
	BConf::serialize(out);
	out << mProductID << mParams;
}

void ProductConf::deserialize(QDataStream &in)
{
	BConf::deserialize(in);
	in >> mProductID >> mParams;
}
