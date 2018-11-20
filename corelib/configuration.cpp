#include "configuration.h"

#include <QDataStream>
#include <algorithm>

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
	{	insertChild(child);	}
}

Configuration::Configuration(std::initializer_list<Configuration> list)
	: Configuration()
{
	for(const Configuration& conf : list)
	{	insertChild(conf);	}
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
	return const_cast<Configuration*>(this)->operator[](childName);
}

Configuration &Configuration::operator[](const QString &childName)
{
	auto rv = std::find_if(mChildren.begin(), mChildren.end(), [childName](const Configuration& conf)
	{
		return conf.name() == childName;
	});
	Q_ASSERT(rv!=mChildren.end());
	return *rv;
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
	{	insertChild(child);	}
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

void Configuration::insertChild(Configuration child, int index)
{
	int insPos = index<0 || index >= mChildren.count() ? mChildren.count() : index;
	mChildren.insert(insPos, child);
	mChildren[insPos].mParent = this;
}

void Configuration::deleteChild(int index)
{
	int delPos = index<0 || index >= mChildren.count() ? mChildren.count() : index;
	mChildren.removeAt(delPos);
}

int Configuration::deleteAllChild(const QString &name)
{
	int rv = 0;
	if(name.isEmpty())
	{
		rv = mChildren.count();
		mChildren.clear();
	}
	else
	{	std::remove_if(mChildren.begin(), mChildren.end(), [name, &rv](const Configuration& conf){
			bool ret = false;
			if(conf.name()==name)
			{
				rv++;
				ret = true;
			}
			return ret;
		});
	}
	return rv;
}

const Configuration *Configuration::childAt(int index) const
{
	const Configuration * rv = nullptr;
	if(index>=0 && index<mChildren.size())
	{	rv = &(mChildren[index]);	}
	return rv;
}

int Configuration::childrenCount() const
{
	return mChildren.size();
}

bool Configuration::containsChild(const QString &childName) const
{
	return std::find_if(mChildren.begin(), mChildren.end(), [childName](const Configuration& conf)
	{
		return conf.name() == childName;
	}) != mChildren.end();
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
		configuration.insertChild(child);
	}
	return in;
}
