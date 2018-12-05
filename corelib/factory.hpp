#pragma once

#include <QSet>
#include <QSharedPointer>
#include <QUuid>
#include <productconf.h>
#include <sharedpointer.hpp>
#include <serial.hpp>
#include <log.h>
#include <tools.h>

class ConfEditorModule;

struct ProductInfo : public SerialInfo
{
	int count;
	QString addr;
};

template <class ProductT, class ConfT>
class Factory
{
public:
	typedef SharedPointer<ProductT> Product;
private:
	Factory(Factory const&) = delete;
	Factory& operator= (Factory const&) = delete;
	Factory(){}
	~Factory(){}
public:
	class Unit
	{
	public:
		Unit(const QString& productName, const ProductID& productID)
			: mProductName(productName)
			, mProductID(productID)
		{
		}
		virtual ~Unit(){}
		virtual ProductT* create(const ConfT& conf) const = 0;
		virtual ConfT* createDefaultConf() const = 0;
		const QString& productName() const					{return mProductName;}
		const ProductID& productID() const					{return mProductID;}
	private:
		QString mProductName;
		ProductID mProductID;
	};

	static Factory& instance();
	Product product(const ConfT &conf);
	bool hasProduct(const ProductID &id) const;
	ProductConf* createDefaultConf(const ProductID &id) const;
	bool registerUnit(Unit *unit);
	ProductList productList() const;
	QList<ProductInfo> info() const;
private:

	QMap<ProductID, QSharedPointer<Unit> > mUnitMap;
	QList<Product> mProducts;
};

template<class ProductT, class ConfT>
Factory<ProductT, ConfT> &Factory<ProductT, ConfT>::instance()
{
	static Factory rv;
	return rv;
}

template<class ProductT, class ConfT>
typename Factory<ProductT, ConfT>::Product Factory<ProductT, ConfT>::product(const ConfT &conf)
{
	Product rv;
	ProductID id = conf.productID();

	if(mUnitMap.contains(id))
	{
		ProductT* p = mUnitMap[id]->create(conf);
		for(const Product& product : mProducts)
		{
			if(product->isSame(*p))
			{
				rv = product;
				delete p;
				break;
			}
		}
		if(rv.isNull())	// продукт не бы найден. нужно создать новый.
		{
			Product pr(p, [this, p](int count){
				if(count == 1)
				{	mProducts.erase(std::find_if(mProducts.begin(), mProducts.end(), [p](const Product& product){return &product==p;}));	}
			});
			mProducts << pr;
			rv = pr;
			rv->populate();
		}
	}

	if(!rv)
	{	Log::error(QString("%1.invalid ProductID: \"%2\"").arg(__CLASS_NAME__).arg(id.toString()));	}
	return rv;
}

template<class ProductT, class ConfT>
bool Factory<ProductT, ConfT>::hasProduct(const ProductID &id) const
{
	return mUnitMap.contains(id);
}

template<class ProductT, class ConfT>
ProductConf *Factory<ProductT, ConfT>::createDefaultConf(const ProductID &id) const
{
	ProductConf* rv;
	if(mUnitMap.contains(id))
	{
		rv = mUnitMap[id]->createDefaultConf();
	}
	return rv;
}

template<class ProductT, class ConfT>
bool Factory<ProductT, ConfT>::registerUnit(Unit* unit)
{
	bool rv = false;
	QSharedPointer<Unit> unitPtr(unit);
	if(! mUnitMap.contains(unit->productID()))
	{
		rv = true;
		mUnitMap[unit->productID()] = unitPtr;
	}
	return rv;
}

template<class ProductT, class ConfT>
ProductList Factory<ProductT, ConfT>::productList() const
{
	ProductList rv;
	for(auto unit : mUnitMap.values())
	{
		rv << QPair<QString, ProductID>(unit->productName(), unit->productID());
	}
	return rv;
}

template<class ProductT, class ConfT>
QList<ProductInfo> Factory<ProductT, ConfT>::info() const
{
	QList<ProductInfo> rv;
	for(const Product& product : mProducts)
	{
		ProductInfo inf;
		SerialInfo pInf = product->info();
		inf.type = pInf.type;
		inf.info = pInf.info;
		inf.count = product.count()-1;
		inf.addr = QString("0x%1").arg(QString::number(reinterpret_cast<ulong>(&product), 16).toUpper());
		rv << inf;
	}
	return rv;
}
