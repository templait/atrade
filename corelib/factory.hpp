#pragma once

#include <QSet>
#include <QSharedPointer>
#include <QUuid>
#include <productconf.h>
#include <sharedpointer.hpp>
#include <log.h>
#include <tools.h>

class ConfEditorModule;

template <class T>
class Factory
{
public:
	typedef SharedPointer<T> Product;
private:
	Factory(Factory const&) = delete;
	Factory& operator= (Factory const&) = delete;
	Factory(){}
	~Factory(){}

	typedef QSharedPointer<ProductConf> ConfPtr;

	struct ProductKey
	{
		ProductKey(const ProductID& id, const ProductConf& cfg) : productID(id), conf(const_cast<ProductConf*>(&cfg)){}
		ProductKey(const ProductKey& other) : productID(other.productID), conf(other.conf.data()){}
		ProductID productID;
		ConfPtr conf;
	};
	class ProductMap
	{
	private:
		class ProductSet
		{
		public:
			bool contains(const ProductConf& key) const;
			bool remove(const ProductConf& key);
			Product  operator [](const ProductConf& key) const;
			Product& operator [](const ProductConf& key);
		private:
			int find(const ProductConf& key) const;
			QList<QPair<ConfPtr, Product> > mList;
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
		virtual T* create(const ProductConf& conf) const = 0;
		virtual ProductConf* createDefaultConf() const = 0;
		const QString& productName() const					{return mProductName;}
		const ProductID& productID() const					{return mProductID;}
	private:
		QString mProductName;
		ProductID mProductID;
	};

	static Factory& instance();
	Product product(const ProductConf& conf);
	bool hasProduct(const ProductID &id) const;
	ProductConf* createDefaultConf(const ProductID &id) const;
	bool registerUnit(Unit *unit);
	ProductList productList() const;
private:

	QMap<ProductID, QSharedPointer<Unit> > mUnitMap; //
	ProductMap mProductMap;	// список созданных продуктов
};

template<class T>
Factory<T> &Factory<T>::instance()
{
	static Factory rv;
	return rv;
}

template<class T>
typename Factory<T>::Product Factory<T>::product(const ProductConf &conf)
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

template<class T>
bool Factory<T>::hasProduct(const ProductID &id) const
{
	return mUnitMap.contains(id);
}

template<class T>
ProductConf *Factory<T>::createDefaultConf(const ProductID &id) const
{
	ProductConf* rv;
	if(mUnitMap.contains(id))
	{
		rv = mUnitMap[id]->createDefaultConf();
	}
	return rv;
}

template<class T>
bool Factory<T>::registerUnit(Unit* unit)
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

template<class T>
ProductList Factory<T>::productList() const
{
	ProductList rv;
	for(auto unit : mUnitMap.values())
	{
		rv << QPair<QString, ProductID>(unit->productName(), unit->productID());
	}
	return rv;
}

template<class T>
bool Factory<T>::ProductMap::ProductSet::contains(const ProductConf &key) const
{
	return find(key)>=0;
}

template<class T>
bool Factory<T>::ProductMap::ProductSet::remove(const ProductConf &key)
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

template<class T>
int Factory<T>::ProductMap::ProductSet::find(const ProductConf &key) const
{
	int rv = -1;
	for(int i=0; i<mList.size(); i++)
	{
		const auto& val = mList[i];
		if(val.first->isSame(key))
		{
			rv = i;
			break;
		}
	}
	return rv;
}

template<class T>
typename Factory<T>::Product Factory<T>::ProductMap::ProductSet::operator [](const ProductConf &key) const
{
	return mList[find(key)].second;
}

template<class T>
typename Factory<T>::Product &Factory<T>::ProductMap::ProductSet::operator [](const ProductConf &key)
{
	int i = find(key);
	if(i>=0)
	{	return mList[find(key)].second;	}
	else
	{
		mList.append({ConfPtr(static_cast<ProductConf*>(key.clone())), Product()});
		return mList.last().second;
	}
}

template<class T>
bool Factory<T>::ProductMap::contains(const Factory<T>::ProductKey &key) const
{
	return mMap.contains(key.productID) && mMap[key.productID].contains(*(key.conf));
}

template<class T>
bool Factory<T>::ProductMap::remove(const Factory<T>::ProductKey &key)
{
	bool rv = false;
	if(mMap.contains(key.productID))
	{	rv = mMap[key.productID].remove(*(key.conf));	}
	return rv;
}

template<class T>
typename Factory<T>::Product Factory<T>::ProductMap::operator [](const Factory<T>::ProductKey &key) const
{
	return mMap[key.productID][*(key.conf)];
}

template<class T>
typename Factory<T>::Product &Factory<T>::ProductMap::operator [](const Factory<T>::ProductKey &key)
{
	return mMap[key.productID][*(key.conf)];
}
