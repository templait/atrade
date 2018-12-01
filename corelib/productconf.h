#pragma once

#include <bconf.h>
#include <types.h>

class ProductConf : public BConf
{
protected:
	ProductConf(const QString& name);
	ProductConf() = delete;
public:
	virtual ~ProductConf() override {}

	ProductID productID() const;
	void setProductID(ProductID productId);
private:
	ProductID mProductID;

	// BConf interface
public:
	virtual bool isSame(const BConf &other) const override;
};
