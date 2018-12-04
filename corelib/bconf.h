#pragma once

#include <QString>

class QDataStream;

/*
проблемы конфигурации:
1. Механизм определения интервалов для источников индикаторов.
2. Каким образом определить может ли родитель содержать потомка данного типа или наоборот.
*/

class BConf
{
	BConf() = delete;
protected:
	BConf(const QString& name);
	BConf(const BConf& other);
public:
	virtual ~BConf();

	const QString& title() const;
	void setTitle(const QString& title);

	const QString& name() const;

	const BConf* parentConf() const;
	bool appendChild(const BConf &conf);
	template<class ParentT> const ParentT* findParent() const;

	virtual bool appendNewChild();
	virtual bool canAppendChild(const BConf& child) const;
	virtual bool insertChild(const BConf &conf, int index);
	virtual int childrenCount() const;
	virtual const BConf* childAt(int index) const;
	virtual void removeChild(int index){Q_UNUSED(index)}
	virtual QString creatableChildName() const;
	virtual void serialize(QDataStream& out) const;
	virtual void deserialize(QDataStream& in);
protected:
	void beParentForChild(BConf *child);
private:
	const BConf* mParent;
	QString mTitle;
	QString mName;
};

template<class ParentT>
const ParentT *BConf::findParent() const
{
	static_assert (std::is_base_of<BConf, ParentT>::value, "ParentT must be subclass of BConf");
	const ParentT* rv = nullptr;
	if(mParent)
	{
		if(const ParentT* p = dynamic_cast<const ParentT*>(mParent))
		{	rv = p;	}
		else
		{	rv = mParent->findParent<ParentT>();	}
	}
	return rv;
}
