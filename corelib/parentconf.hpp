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
	virtual bool isSame(const BConf& other) const override;
	//virtual void serialize(QDataStream& out) const;
	//virtual void deserialize(QDataStream& in);
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
		ParentT::beParentForChild(&mChildren[index]);
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

template<class ParentT, class ChildT>
bool ParentConf<ParentT, ChildT>::isSame(const BConf &other) const
{
	bool rv = false;
	if(const ParentConf<ParentT, ChildT>* ptrOther = dynamic_cast<const ParentConf<ParentT, ChildT>*>(&other))
	{
		rv = ptrOther->isSame(other);
		if(rv)
		{
			for(const ChildT& child : mChildren)
			{
				rv = child.isSame(*ptrOther);
				if(!rv) break;
			}
		}
	}
	return rv;
}
