#pragma once

#include <QSet>
#include <QSharedPointer>
#include <QUuid>
#include <productconf.h>
#include <sharedpointer.hpp>
#include <log.h>
#include <tools.h>

class ConfEditorModule;

template <class dsT, class confT>
class Factory
{
public:
	typedef SharedPointer<dsT> Product;
private:
	Factory(Factory const&) = delete;
	Factory& operator= (Factory const&) = delete;
	Factory(){}
	~Factory(){}

	struct ProductKey
	{
		ProductID productID;
		confT conf;
	};
	class ProductMap
	{
	private:
		class ProductSet
		{
		public:
			bool contains(const confT& key) const;
			bool remove(const confT& key);
			Product  operator [](const confT& key) const;
			Product& operator [](const confT& key);
		private:
			int find(const confT& key) const;
			QList<QPair<confT, Product> > mList;
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
		virtual dsT* create(const confT& conf) const = 0;
		virtual confT* createDefaultConf() const = 0;
		const QString& productName() const					{return mProductName;}
		const ProductID& productID() const					{return mProductID;}
	private:
		QString mProductName;
		ProductID mProductID;
	};

	static Factory& instance();
	Product product(const confT &conf);
	bool hasProduct(const ProductID &id) const;
	ProductConf* createDefaultConf(const ProductID &id) const;
	bool registerUnit(Unit *unit);
	ProductList productList() const;
private:

	QMap<ProductID, QSharedPointer<Unit> > mUnitMap; //
	ProductMap mProductMap;	// список созданных продуктов
};

template<class dsT, class confT>
Factory<dsT, confT> &Factory<dsT, confT>::instance()
{
	static Factory rv;
	return rv;
}

template<class dsT, class confT>
typename Factory<dsT, confT>::Product Factory<dsT, confT>::product(const confT &conf)
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

template<class dsT, class confT>
bool Factory<dsT, confT>::hasProduct(const ProductID &id) const
{
	return mUnitMap.contains(id);
}

template<class dsT, class confT>
ProductConf *Factory<dsT, confT>::createDefaultConf(const ProductID &id) const
{
	ProductConf* rv;
	if(mUnitMap.contains(id))
	{
		rv = mUnitMap[id]->createDefaultConf();
	}
	return rv;
}

template<class dsT, class confT>
bool Factory<dsT, confT>::registerUnit(Unit* unit)
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

template<class dsT, class confT>
ProductList Factory<dsT, confT>::productList() const
{
	ProductList rv;
	for(auto unit : mUnitMap.values())
	{
		rv << QPair<QString, ProductID>(unit->productName(), unit->productID());
	}
	return rv;
}

template<class dsT, class confT>
bool Factory<dsT, confT>::ProductMap::ProductSet::contains(const confT &key) const
{
	return find(key)>=0;
}

template<class dsT, class confT>
bool Factory<dsT, confT>::ProductMap::ProductSet::remove(const confT &key)
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

template<class dsT, class confT>
int Factory<dsT, confT>::ProductMap::ProductSet::find(const confT &key) const
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

template<class dsT, class confT>
typename Factory<dsT, confT>::Product Factory<dsT, confT>::ProductMap::ProductSet::operator [](const confT &key) const
{
	return mList[find(key)].second;
}

template<class dsT, class confT>
typename Factory<dsT, confT>::Product &Factory<dsT, confT>::ProductMap::ProductSet::operator [](const confT &key)
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

template<class dsT, class confT>
bool Factory<dsT, confT>::ProductMap::contains(const Factory<dsT, confT>::ProductKey &key) const
{
	return mMap.contains(key.productID) && mMap[key.productID].contains(key.conf);
}

template<class dsT, class confT>
bool Factory<dsT, confT>::ProductMap::remove(const Factory<dsT, confT>::ProductKey &key)
{
	bool rv = false;
	if(mMap.contains(key.productID))
	{	rv = mMap[key.productID].remove(key.conf);	}
	return rv;
}

template<class dsT, class confT>
typename Factory<dsT, confT>::Product Factory<dsT, confT>::ProductMap::operator [](const Factory<dsT, confT>::ProductKey &key) const
{
	return mMap[key.productID][key.conf];
}

template<class dsT, class confT>
typename Factory<dsT, confT>::Product &Factory<dsT, confT>::ProductMap::operator [](const Factory<dsT, confT>::ProductKey &key)
{
	return mMap[key.productID][key.conf];
}
