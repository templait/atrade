#include "bconf.h"

#include <QChildEvent>
#include <QMetaProperty>

BConf::BConf(BConf *parent) : QObject(parent)
{

}

BConf::~BConf()
{
	qDeleteAll(mChildTypes);
}

const QString &BConf::title() const
{
	return mTitle;
}

void BConf::setTitle(const QString &title)
{
	mTitle = title;
	emit titleChanged(mTitle);
}

const QString &BConf::name() const
{
	return mName;
}

void BConf::setName(const QString &name)
{
	mName=name;
	emit nameChanged(mName);
}

BConf *BConf::parentConf()
{
	BConf* parentCfg = qobject_cast<BConf*>(parent());
	Q_CHECK_PTR(parentCfg);
	return parentCfg;
}

bool BConf::canAppendChild(const BConf *child) const
{
	bool rv=false;
	for(const BConf* childType : mChildTypes)
	{
		if(childType->metaObject()->className() == child->metaObject()->className())
		{
			rv = true;
			break;
		}
	}
	return rv;
}

BConf *BConf::clone(BConf *parentConf) const
{
	const QMetaObject* met = metaObject();
	BConf * rv = qobject_cast<BConf*>(met->newInstance(Q_ARG(BConf*, parentConf)));

	for(int i=0; i<met->propertyCount(); i++)
	{
		QMetaProperty prop = met->property(i);
		prop.write(rv, prop.read(this));
	}

	for(const QObject* obj : children())
	{
		const BConf* child = qobject_cast<const BConf*>(obj);
		Q_CHECK_PTR(child);
		child->clone(rv);
	}
	return rv;
}

bool BConf::insertChild(int index, BConf *conf)
{
	bool rv = canAppendChild(conf);
	if(rv)
	{
		conf->setParent(this);
		mChildren.insert(index, conf);
	}
	return rv;
}

bool BConf::insertChild(int index, int typeIndex)
{
	bool rv=false;
	if(childTypesCount())
	{
		typeIndex = qBound(0, typeIndex, childTypesCount()-1);
		rv = insertChild(index, mChildCtors[index](this));
	}
	return rv;
}

bool BConf::appendChild(BConf *conf)
{
	return insertChild(mChildren.count(), conf);
}

bool BConf::appendChild(int typeIndex)
{
	return insertChild(mChildren.count(), typeIndex);
}

int BConf::childTypesCount() const
{
	return mChildTypes.count();
}

int BConf::childrenCount() const
{
	return mChildren.count();
}

QStringList BConf::childTypesNames() const
{
	QStringList rv;
	for(const BConf* childType : mChildTypes)
	{
		rv << childType->name();
	}
	return rv;
}

BConf *BConf::childAt(int index)
{
	BConf* rv=nullptr;
	if(index>=0 && index<=mChildren.size())
	{	rv = mChildren[index];	}
	return rv;
}

void BConf::appendChildctor(BConf::Childctor childctor)
{
	mChildTypes << childctor(nullptr);
	mChildCtors << childctor;
}

bool BConf::event(QEvent *event)
{
	if(event->type() == QEvent::ChildAdded)
	{
		QChildEvent* chEvent = static_cast<QChildEvent*>(event);
		BConf* child = qobject_cast<BConf*>(chEvent->child());
		Q_CHECK_PTR(child);
		Q_ASSERT(canAppendChild(child));
		mChildren << child;
	}
	else if(event->type() == QEvent::ChildRemoved)
	{
		QChildEvent* chEvent = static_cast<QChildEvent*>(event);
		BConf* child = qobject_cast<BConf*>(chEvent->child());
		Q_CHECK_PTR(child);
		mChildren.removeAll(child);
	}
	return QObject::event(event);
}
