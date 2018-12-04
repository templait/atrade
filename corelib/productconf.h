#pragma once

#include <QVariantMap>
#include <bconf.h>
#include <types.h>

class ProductConf : public BConf
{
	ProductConf() = delete;
protected:
	ProductConf(const QString& name);
public:
	virtual ~ProductConf() override {}

	ProductID productID() const;
	void setProductID(ProductID productId);

	void setParam(const QString& name, const QVariant& value);
	QVariant param(const QString& name) const;
private:
	ProductID mProductID;
	QVariantMap mParams;

	// BConf interface
public:
	virtual void serialize(QDataStream &out) const override;
	virtual void deserialize(QDataStream &in) override;
};
