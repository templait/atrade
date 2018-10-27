#pragma once

#include <QSet>
#include <QSharedPointer>
#include <QUuid>
#include <QVariantMap>
#include <sharedpointer.hpp>
#include <log.h>
#include <tools.h>

template <class T>
class Factory
{
public:
	typedef SharedPointer<T> Product;
	typedef QUuid UnitID;
	typedef QUuid ProductID;
private:
	Factory(Factory const&) = delete;
	Factory& operator= (Factory const&) = delete;
	Factory(){}
	~Factory(){}
	struct ProductDescr
	{
		ProductID id;
		QString name;
	};
	struct ProductKey
	{
		ProductID productID;
		QVariantMap settings;
	};
	class ProductMap
	{
	private:
		class ProductList
		{
		public:
			bool contains(const QVariantMap& key) const;
			bool remove(const QVariantMap& key);
			Product  operator [](const QVariantMap& key) const;
			Product& operator [](const QVariantMap& key);
		private:
			int find(const QVariantMap& key) const;
			QList<QPair<QVariantMap, Product> > mList;
		};
		QMap<ProductID,  ProductList> mMap;

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
		Unit(const QString& unitName, const UnitID& id, const QList<ProductDescr> & productList)
		    : mUnitName(unitName)
		    , mID(id)
		{
			for(const ProductDescr& descr : productList)
			{	mProductsDescr[descr.id] = descr; }
		}
		virtual ~Unit(){}
		virtual T* create(const ProductID& id, const QVariantMap& setiings) const = 0;
		const QString& unitName() const					{return mUnitName;}
		UnitID unitID() const							{return mID;}
		QList<ProductDescr> productList() const			{return mProductsDescr.values();}
		QString productName(const ProductID& productID) const
		{
			QString rv;
			if(mProductsDescr.contains(productID))
			{	rv = mProductsDescr[productID].name;	}
			return rv;
		}
	private:
		QString mUnitName;
		UnitID mID;
		QMap<ProductID, ProductDescr> mProductsDescr;
	};


	static Factory& instance();
	Product get(const ProductID &id, const QVariantMap& settings = QVariantMap());
	bool registerUnit(Unit *unit);
private:

	QMap<ProductID, QSharedPointer<Unit> > mUnitMap;
	ProductMap mProductMap;
	QSet<QSharedPointer<Unit> > mUnits;
};

template<class T>
Factory<T> &Factory<T>::instance()
{
	static Factory rv;
	return rv;
}

template<class T>
typename Factory<T>::Product Factory<T>::get(const Factory<T>::ProductID &id, const QVariantMap &settings)
{
	Product rv;
	ProductKey productKey = {id, settings};
	if(!mProductMap.contains(productKey))
	{
		if(mUnitMap.contains(id))
		{
			rv = Product(mUnitMap[id]->create(id, settings), [this, productKey](int count){
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
bool Factory<T>::registerUnit(Unit* unit)
{
	bool rv = false;
	QSharedPointer<Unit> unitPtr(unit);
	if(! mUnits.contains(unitPtr))
	{
		mUnits << unitPtr;
		rv = true;
		for(const ProductDescr& productDescr : unitPtr->productList())
		{
			mUnitMap[productDescr.id] = unitPtr;
		}
	}
	return rv;
}

template<class T>
bool Factory<T>::ProductMap::ProductList::contains(const QVariantMap &key) const
{
	return find(key)>=0;
}

template<class T>
bool Factory<T>::ProductMap::ProductList::remove(const QVariantMap &key)
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
int Factory<T>::ProductMap::ProductList::find(const QVariantMap &key) const
{
	int rv = -1;
	for(int i=0; i<mList.size(); i++)
	{
		const auto& val = mList[i];
		if(val.first == key)
		{
			rv = i;
			break;
		}
	}
	return rv;
}

template<class T>
typename Factory<T>::Product Factory<T>::ProductMap::ProductList::operator [](const QVariantMap &key) const
{
	return mList[find(key)].second;
}

template<class T>
typename Factory<T>::Product &Factory<T>::ProductMap::ProductList::operator [](const QVariantMap &key)
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

template<class T>
bool Factory<T>::ProductMap::contains(const Factory<T>::ProductKey &key) const
{
	return mMap.contains(key.productID) && mMap[key.productID].contains(key.settings);
}

template<class T>
bool Factory<T>::ProductMap::remove(const Factory<T>::ProductKey &key)
{
	bool rv = false;
	if(mMap.contains(key.productID))
	{	rv = mMap[key.productID].remove(key.settings);	}
	return rv;
}

template<class T>
typename Factory<T>::Product Factory<T>::ProductMap::operator [](const Factory<T>::ProductKey &key) const
{
	return mMap[key.productID][key.settings];
}

template<class T>
typename Factory<T>::Product &Factory<T>::ProductMap::operator [](const Factory<T>::ProductKey &key)
{
	return mMap[key.productID][key.settings];
}
