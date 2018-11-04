#include "configuration.h"

#include <QDataStream>

Configuration::Configuration()
	: mParent(nullptr)
    , mUserEditableMap(ReadOnly)
{

}

Configuration::Configuration(const Configuration &other)
	: Configuration()
{
	mName = other.mName;
	mValue = other.mValue;
	mTitle = other.mTitle;
	mUserEditableMap = other.mUserEditableMap;

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

Configuration::Configuration(int userEditableMap, const QString &name, const QVariant &value, const QString &title)
    : Configuration(name, value, title)
{
	mUserEditableMap = userEditableMap;
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
	// название не учавствует в сравнении т.к. это приведёт к некорректной рабте фабрик.
	// дело в том что конфигурация является ключом определяющим необходимость создания нового продукта.
	// учитывая то, что одни и те же продукты могут иметь разные названия, фабрика будет создавать всё новые продукты.
	if(childrenCount()==other.childrenCount() && mName==other.mName && mValue==other.mValue /*&& mTitle==other.mTitle*/)
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
	mUserEditableMap = other.mUserEditableMap;
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
	/*if(! mChildren.contains(child.name())) // Убрано т.к. необходимо держать в конфигурации несколько источников с одним именем
	{
		mChildren[child.name()] = child;
		mChildren[child.name()].mParent = this;
		rv = true;
	}*/
	(mChildren.insertMulti(child.name(), child))->mParent = this;
	return rv;
}

const Configuration *Configuration::childAt(int index) const
{
	const Configuration* rv = nullptr;
	if(index<mChildren.size())
	{
		rv = (mChildren.cbegin()+index).operator->();
	}
	return rv;
}

int Configuration::childrenCount() const
{
	return mChildren.size();
}

bool Configuration::containsChild(const QString &childName) const
{
	return mChildren.contains(childName);
}

bool Configuration::userEditabe(Configuration::EParam param) const
{
	return mUserEditableMap & param;
}

void Configuration::setUserEditableMap(int map)
{
	mUserEditableMap = map;
}

Configuration *Configuration::parent()
{
	return mParent;
}

QDataStream &operator<<(QDataStream &out, const Configuration &configuration)
{
	int childCount = configuration.mChildren.size();
	out << configuration.mName << configuration.mValue << configuration.mTitle << configuration.mUserEditableMap << childCount;
	for(int i=0; i<childCount; i++)
	{	out << *(const_cast<Configuration&>(configuration).childAt(i));	}
	return out;
}

QDataStream &operator>>(QDataStream &in, Configuration &configuration)
{
	int childCount;
	in >> configuration.mName >> configuration.mValue >> configuration.mTitle >> configuration.mUserEditableMap >> childCount;
	for(int i=0; i<childCount; i++)
	{
		Configuration child;
		in >> child;
		configuration.appendChild(child);
	}
	return in;
}
