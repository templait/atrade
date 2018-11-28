#pragma once

#include <QMap>
#include <QMetaType>

/*
проблемы конфигурации:
1. Механизм определения интервалов для источников индикаторов.
2. Каким образом определить может ли родитель содержать потомка данного типа или наоборот.
*/


class BConf
{
public:
	BConf(const QString& name);
	virtual ~BConf();

	const QString& title() const;
	void setTitle(const QString& title);

	const QString& name() const;

	const BConf* parentConf();
	bool appendChild(const BConf &conf);
	virtual bool appendNewChild();
	virtual bool canAppendChild(const BConf& child) const;
	virtual bool insertChild(const BConf &conf, int index);
	virtual int childrenCount() const;
	virtual const BConf* childAt(int index);
	virtual bool removeChild(int index);
	virtual QString childName() const;
public:
	void setParent(const BConf* parent);

private:
	const BConf* mParent;
	QString mTitle;
	QString mName;
};
