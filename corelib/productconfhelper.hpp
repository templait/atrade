#pragma once

#include <productconf.h>

template<class ConfT>
class ProductConfHelper
{
	static_assert (std::is_base_of<ProductConf, ConfT>::value, "ConfT must be subclass of ProductConf");
public:
	ProductConfHelper(ProductID id, ConfT* conf=nullptr)
		: mConf(conf)
		, mProductID(id)
	{	}
	virtual ~ProductConfHelper(){}

	const ConfT& conf() const				{	return *mConf;	}
	ConfT& conf()							{	return *mConf;	}
	void setConf(ConfT* conf)				{	mConf=conf;	}
	const ProductID& productID() const		{	return mProductID;	}

	virtual bool isValid() const			{	return mConf && mConf->productID()==mProductID;	}
	virtual ConfT* createConf() = 0;
protected:
	template<typename ParamT> ParamT getParam(const QString& paramName) const
	{
		QString rv;
		if(isValid())
		{	rv = conf().param(paramName).template value<ParamT>();	}
		return rv;
	}
	template<typename ParamT> void setParam(const QString& paramName, const ParamT& value)
	{
		if(isValid())
		{	conf().setParam(paramName, value);	}
	}
private:
	ConfT* mConf;
	ProductID mProductID;
};

