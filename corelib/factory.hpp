#pragma once

#include <QSet>
#include <QSharedPointer>
#include <QUuid>
#include <configuration.h>
#include <sharedpointer.hpp>
#include <log.h>
#include <tools.h>

typedef QUuid ProductID;
typedef QList<QPair<QString, ProductID> > ProductList;

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

	struct ProductKey
	{
		ProductID productID;
		Configuration settings;
	};
	class ProductMap
	{
	private:
		class ProductSet
		{
		public:
			bool contains(const Configuration& key) const;
			bool remove(const Configuration& key);
			Product  operator [](const Configuration& key) const;
			Product& operator [](const Configuration& key);
		private:
			int find(const Configuration& key) const;
			QList<QPair<Configuration, Product> > mList;
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
		virtual T* create(const ProductID& id, const Configuration& setiings) const = 0;
		//virtual Configuration defaultConfiguration(const ProductID& id) const = 0;
		const QString& productName() const					{return mProductName;}
		const ProductID& productID() const					{return mProductID;}
	private:
		QString mProductName;
		ProductID mProductID;
	};


	static Factory& instance();
	Product get(const ProductID &id, const Configuration& settings = Configuration());
	bool registerUnit(Unit *unit);
	ProductList productList() const;
private:

	QMap<ProductID, QSharedPointer<Unit> > mUnitMap;
	ProductMap mProductMap;
};

template<class T>
Factory<T> &Factory<T>::instance()
{
	static Factory rv;
	return rv;
}

template<class T>
typename Factory<T>::Product Factory<T>::get(const ProductID &id, const Configuration &settings)
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
bool Factory<T>::ProductMap::ProductSet::contains(const Configuration &key) const
{
	return find(key)>=0;
}

template<class T>
bool Factory<T>::ProductMap::ProductSet::remove(const Configuration &key)
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
int Factory<T>::ProductMap::ProductSet::find(const Configuration &key) const
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
typename Factory<T>::Product Factory<T>::ProductMap::ProductSet::operator [](const Configuration &key) const
{
	return mList[find(key)].second;
}

template<class T>
typename Factory<T>::Product &Factory<T>::ProductMap::ProductSet::operator [](const Configuration &key)
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
