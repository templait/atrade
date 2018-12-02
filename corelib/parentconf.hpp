#pragma once

#include "bconf.h"
#include <QList>
#include <QVBoxLayout>
#include <luaconf.h>
#include <type_traits>

template<class ParentT, class ChildT>
class ParentConf : public ParentT
{
	static_assert(std::is_base_of<BConf, ParentT>::value, "ParentT must be subclass of BConf");
	static_assert(std::is_base_of<BConf, ChildT>::value, "ChildT must be subclass of BConf");
	ParentConf() = delete;
protected:
	ParentConf(const QString& name) : ParentT(name){}
	ParentConf(const ParentConf& other);
public:
	virtual ~ParentConf() override {}
private:
	QList<ChildT> mChildren;
protected:
	const QList<ChildT>& children() const;
	// BConf interface
public:
	virtual bool canAppendChild(const BConf &child) const override;
	virtual bool insertChild(const BConf &conf, int index) override;
	virtual int childrenCount() const override;
	virtual const BConf *childAt(int index) const override;
	virtual void removeChild(int index) override;
	//virtual bool isSame(const BConf& other) const override;
	virtual void serialize(QDataStream& out) const override;
	virtual void deserialize(QDataStream& in) override;
};


template<class ParentT, class ChildT>
ParentConf<ParentT, ChildT>::ParentConf(const ParentConf &other) : ParentT(other)
{
	mChildren = other.mChildren;
	for(ChildT& child : mChildren)
	{	this->beParentForChild(&child);	}
}

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
		this->beParentForChild(&mChildren[index]);
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
const BConf *ParentConf<ParentT, ChildT>::childAt(int index) const
{
	const BConf * rv = nullptr;
	if(index>=0 && index<mChildren.size())
	{	rv = &mChildren[index];	}
	return rv;
}

template<class ParentT, class ChildT>
void ParentConf<ParentT, ChildT>::removeChild(int index)
{
	Q_ASSERT(index>=0 && index<mChildren.count());
	mChildren.removeAt(index);
}
/*
template<class ParentT, class ChildT>
bool ParentConf<ParentT, ChildT>::isSame(const BConf &other) const
{
	bool rv = false;
	if(const ParentConf<ParentT, ChildT>* ptrOther = dynamic_cast<const ParentConf<ParentT, ChildT>*>(&other))
	{
		rv = ParentT::isSame(*ptrOther);
		if(rv && mChildren.count()==ptrOther->childrenCount())
		{
			for(int i=0; i<mChildren.count(); i++)
			{
				rv = mChildren[i].isSame(ptrOther->mChildren[i]);
				if(!rv) break;
			}
		}
	}
	return rv;
}*/

template<class ParentT, class ChildT>
void ParentConf<ParentT, ChildT>::serialize(QDataStream &out) const
{
	ParentT::serialize(out);
	out << mChildren.count();
	for(const ChildT& child : mChildren)
	{	child.serialize(out);	}
}

template<class ParentT, class ChildT>
void ParentConf<ParentT, ChildT>::deserialize(QDataStream &in)
{
	ParentT::deserialize(in);
	int chCount;
	in >> chCount;
	for(int i=0; i<chCount; i++)
	{
		ChildT child;
		child.deserialize(in);
		this->appendChild(child);
	}
}
