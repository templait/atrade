#pragma once

#include <parentconf.hpp>

template<class ParentT, class ChildT>
class AddChildParentConf : public ParentConf<ParentT, ChildT>
{
	AddChildParentConf()=delete;
public:
	virtual ~AddChildParentConf() override{}
protected:
	AddChildParentConf(const QString& name) : ParentConf<ParentT, ChildT>(name){}

	// BConf interface
public:
	virtual bool appendNewChild() override;
	virtual QString creatableChildName() const override;
};

template<class ParentT, class ChildT>
bool AddChildParentConf<ParentT, ChildT>::appendNewChild()
{
	return ParentT::appendChild(ChildT());
}

template<class ParentT, class ChildT>
QString AddChildParentConf<ParentT, ChildT>::creatableChildName() const
{
	return ChildT().name();
}
