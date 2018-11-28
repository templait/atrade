#pragma once

#include "bconf.h"
#include <QList>

template<class ParentT, class ChildT>
class ParentConf : public ParentT
{
	static_assert(std::is_base_of_v<BConf, ParentT>, "ParentT must be subclass of BConf");
	static_assert(std::is_base_of_v<BConf, ChildT>, "ChildT must be subclass of BConf");
public:
	ParentConf(const QString& name) : ParentT(name){}
	virtual ~ParentConf() override{}
private:
	QList<ChildT> mChildren;
	// BConf interface
public:
	virtual bool canAppendChild(const BConf &child) const override;
	virtual bool insertChild(const BConf &conf, int index) override;
	virtual int childrenCount() const override;
	virtual const BConf *childAt(int index) override;
	virtual bool removeChild(int index) override;
	virtual bool appendNewChild() override;
	virtual QString childName() const override;
};


template<class ParentT, class ChildT>
bool ParentConf<ParentT, ChildT>::canAppendChild(const BConf &child) const
{
	return dynamic_cast<const ChildT*>(&child);
}

template<class ParentT, class ChildT>
bool ParentConf<ParentT, ChildT>::insertChild(const BConf &conf, int index)
{
	bool rv = false;
	if(canAppendChild(conf))
	{
		index = qBound(0, index, mChildren.count());
		mChildren.insert(index, static_cast<const ChildT&>(conf));
		ParentT::beParentForChild(mChildren[index]);
		rv = true;
	}
	return rv;
}

template<class ParentT, class ChildT>
int ParentConf<ParentT, ChildT>::childrenCount() const
{
	return mChildren.count();
}

template<class ParentT, class ChildT>
const BConf *ParentConf<ParentT, ChildT>::childAt(int index)
{
	const BConf* rv = nullptr;
	if(index>=0 && index<mChildren.size())
	{	rv = &mChildren[index];	}
	return rv;
}

template<class ParentT, class ChildT>
bool ParentConf<ParentT, ChildT>::removeChild(int index)
{
	bool rv = false;
	if(index>=0 && index<mChildren.count())
	{
		mChildren.removeAt(index);
	}
	return rv;
}

template<class ParentT, class ChildT>
bool ParentConf<ParentT, ChildT>::appendNewChild()
{
	return ParentT::appendChild(ChildT());
}

template<class ParentT, class ChildT>
QString ParentConf<ParentT, ChildT>::childName() const
{
	return ChildT().name();
}
