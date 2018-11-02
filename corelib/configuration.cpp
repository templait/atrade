#include "configuration.h"

#include <QDataStream>

Parameter::Parameter(const QVariant &value, const QString &title)
	: mValue(value)
	, mTitle(title)
{

}

void Parameter::setTitle(const QString &title)
{
	mTitle = title;
}

const QString &Parameter::title() const
{
	return mTitle;
}

void Parameter::setValue(const QVariant &value)
{
	mValue = value;
}

const QVariant &Parameter::value() const
{
	return mValue;
}

bool Parameter::operator==(const Parameter &other) const
{
	return mTitle==other.mTitle && mValue==other.mValue;
}

Configuration::Configuration()
	: mParent(nullptr)
{

}

Configuration::Configuration(const Configuration &other)
	: Configuration()
{
	mName = other.mName;
	mValue = other.mValue;
	mTitle = other.mTitle;

	for(const Configuration& child : other.mChildren)
	{	appendChild(child);	}
}

Configuration::Configuration(std::initializer_list<Configuration> list)
	: Configuration()
{
	for(const Configuration& conf : list)
	{	appendChild(conf);	}
}

Configuration::Configuration(const QString &name, const QVariant &value, const QString &title)
	: mName(name)
	, mValue(value)
	, mTitle(title)
	, mParent(nullptr)
{

}

Configuration Configuration::operator[](const QString &childName) const
{
	return mChildren[childName];
}

Configuration &Configuration::operator[](const QString &childName)
{
	return mChildren[childName];
}

bool Configuration::operator==(const Configuration &other) const
{
	bool rv = false;
	if(childrenCount()==other.childrenCount() && mName==other.mName && mValue==other.mValue && mTitle==other.mTitle)
	{
		rv=true;
		if(childrenCount()>0)
		{	rv = mChildren==other.mChildren;	}
	}
	return rv;
}

Configuration &Configuration::operator=(const Configuration &other)
{
	mName = other.mName;
	mValue = other.mValue;
	mTitle = other.mTitle;
	mChildren.clear();
	for(const Configuration& child : other.mChildren)
	{	appendChild(child);	}
	return *this;
}

const QString &Configuration::name() const
{
	return mName;
}

void Configuration::setName(const QString &name)
{
	mName = name;
}

const QVariant &Configuration::value() const
{
	return mValue;
}

void Configuration::setValue(const QVariant &value)
{
	mValue = value;
}

const QString &Configuration::title() const
{
	return mTitle;
}

void Configuration::setTitle(const QString &title)
{
	mTitle = title;
}

bool Configuration::appendChild(Configuration child)
{
	bool rv = false;
	if(! mChildren.contains(child.name()))
	{
		mChildren[child.name()] = child;
		mChildren[child.name()].mParent = this;
		rv = true;
	}
	return rv;
}

Configuration* Configuration::childAt(int index)
{
	Configuration* rv = nullptr;
	if(index<mChildren.size())
	{
		rv = (mChildren.begin()+index).operator->();
	}
	return rv;
}

int Configuration::childrenCount() const
{
	return mChildren.size();
}

Configuration *Configuration::parent()
{
	return mParent;
}

QDataStream &operator<<(QDataStream &out, const Configuration &configuration)
{
	int childCount = configuration.mChildren.size();
	out << configuration.mName << configuration.mValue << configuration.mTitle << childCount;
	for(int i=0; i<childCount; i++)
	{	out << const_cast<Configuration&>(configuration).childAt(i);	}
	return out;
}

QDataStream &operator>>(QDataStream &in, Configuration &configuration)
{
	int childCount;
	in >> configuration.mName >> configuration.mValue >> configuration.mTitle >> childCount;
	for(int i=0; i<childCount; i++)
	{
		Configuration child;
		in >> child;
		configuration.appendChild(child);
	}
	return in;
}
