#pragma once

#include <QSet>
#include <QSharedPointer>
#include <QUuid>
#include <QVariantMap>
#include <sharedpointer.hpp>

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
	QMap<ProductID, Product> mProductMap;
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
	if(!mProductMap.contains(id))
	{
		rv = Product(mUnitMap[id]->create(id, settings), [this, id](int count){
			if(count == 1)
			{	mProductMap.remove(id);	}
		});
		mProductMap[id] = rv;
	}
	else
	{	rv = mProductMap[id];	}
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
