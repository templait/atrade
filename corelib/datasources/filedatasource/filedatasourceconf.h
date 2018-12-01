#pragma once

#include <datasourceconf.h>

class FileDataSourceConf final : public DataSourceConf
{
public:
	FileDataSourceConf(const ProductID& id, const QString& className, const QString& code);

	const QString& className() const;
	void setClassName(const QString& className);

	const QString& code() const;
	void setCode(const QString& code);

private:
	QString mClassName;
	QString mCode;

	// BConf interface
public:
	virtual bool isSame(const BConf &other) const override;
	virtual BConf *clone() const override;
};
