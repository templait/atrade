#pragma once

#include <QSet>
#include <QSharedPointer>
#include <QUuid>
#include <productconf.h>
#include <sharedpointer.hpp>
#include <log.h>
#include <tools.h>

class ConfEditorModule;

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

	struct ProductKey
	{
		ProductID productID;
		ConfT conf;
	};
	class ProductMap
	{
	private:
		class ProductSet
		{
		public:
			bool contains(const ConfT& key) const;
			bool remove(const ConfT& key);
			Product  operator [](const ConfT& key) const;
			Product& operator [](const ConfT& key);
		private:
			int find(const ConfT& key) const;
			QList<QPair<ConfT, Product> > mList;
		};
		QMap<ProductID,  ProductSet> mMap;

	public:
		bool contains(const ProductKey& key) const;
		bool remove(const ProductKey& key);
		Product  operator [](const ProductKey& key) const;
		Product& operator [](const ProductKey& key);
	};
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
private:

	QMap<ProductID, QSharedPointer<Unit> > mUnitMap; //
	ProductMap mProductMap;	// список созданных продуктов
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
	ProductKey productKey = {id, conf};
	if(!mProductMap.contains(productKey))
	{
		if(mUnitMap.contains(id))
		{
			rv = Product(mUnitMap[id]->create(conf), [this, productKey](int count){
				if(count == 1)
				{	mProductMap.remove(productKey);	}
			});
			mProductMap[productKey] = rv;
		}
	}
	else
	{	rv = mProductMap[productKey];	}

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
bool Factory<ProductT, ConfT>::ProductMap::ProductSet::contains(const ConfT &key) const
{
	return find(key)>=0;
}

template<class ProductT, class ConfT>
bool Factory<ProductT, ConfT>::ProductMap::ProductSet::remove(const ConfT &key)
{
	bool rv = false;
	int i = find(key);
	if(i>=0)
	{
		mList.removeAt(i);
		rv = true;
	}
	return rv;
}

template<class ProductT, class ConfT>
int Factory<ProductT, ConfT>::ProductMap::ProductSet::find(const ConfT &key) const
{
	int rv = -1;
	for(int i=0; i<mList.size(); i++)
	{
		const auto& val = mList[i];
		if(val.first.isSame(key))
		{
			rv = i;
			break;
		}
	}
	return rv;
}

template<class ProductT, class ConfT>
typename Factory<ProductT, ConfT>::Product Factory<ProductT, ConfT>::ProductMap::ProductSet::operator [](const ConfT &key) const
{
	return mList[find(key)].second;
}

template<class ProductT, class ConfT>
typename Factory<ProductT, ConfT>::Product &Factory<ProductT, ConfT>::ProductMap::ProductSet::operator [](const ConfT &key)
{
	int i = find(key);
	if(i>=0)
	{	return mList[find(key)].second;	}
	else
	{
		mList.append({key, Product()});
		return mList.last().second;
	}
}

template<class ProductT, class ConfT>
bool Factory<ProductT, ConfT>::ProductMap::contains(const Factory<ProductT, ConfT>::ProductKey &key) const
{
	return mMap.contains(key.productID) && mMap[key.productID].contains(key.conf);
}

template<class ProductT, class ConfT>
bool Factory<ProductT, ConfT>::ProductMap::remove(const Factory<ProductT, ConfT>::ProductKey &key)
{
	bool rv = false;
	if(mMap.contains(key.productID))
	{	rv = mMap[key.productID].remove(key.conf);	}
	return rv;
}

template<class ProductT, class ConfT>
typename Factory<ProductT, ConfT>::Product Factory<ProductT, ConfT>::ProductMap::operator [](const Factory<ProductT, ConfT>::ProductKey &key) const
{
	return mMap[key.productID][key.conf];
}

template<class ProductT, class ConfT>
typename Factory<ProductT, ConfT>::Product &Factory<ProductT, ConfT>::ProductMap::operator [](const Factory<ProductT, ConfT>::ProductKey &key)
{
	return mMap[key.productID][key.conf];
}
