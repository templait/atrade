#pragma once

#include <bconf.h>
#include <factory.hpp>

class ProductConf : public BConf
{
public:
	ProductConf(const QString& name=QString());
	virtual ~ProductConf() override {}

	ProductID productID() const;
	void setProductID(ProductID productId);
private:
	ProductID mProductID;

	// BConf interface
public:
	virtual bool isSame(const BConf &other) const override;
};
