#include "bconf.h"
#include <log.h>
#include <tools.h>

#include <QChildEvent>
#include <QMetaProperty>

QMap<QString, const QMetaObject*> BConf::mRegistreadConfs;

BConf::BConf() : QObject(nullptr)
{

}

BConf::~BConf()
{
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

bool BConf::isConfRegistred(const BConf *conf)
{
	return mRegistreadConfs.contains(conf->metaObject()->className());
}

BConf *BConf::parentConf()
{
	BConf* rv = nullptr;
	if(QObject * pObject = parent())
	{
		rv = qobject_cast<BConf*>(pObject);
		Q_CHECK_PTR(rv);
	}
	return rv;
}

bool BConf::canAppendChild(const BConf *child) const
{
	bool rv=false;
	for(const QMetaObject* childMeta : mMetaChildren)
	{
		if(childMeta->className() == child->metaObject()->className())
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
	Q_UNUSED(index)
	Q_UNUSED(conf)
	/*
	bool rv = canAppendChild(conf);
	if(rv && conf->parent()!=this)
	{
		conf->setParent(this);
		mChildren.insert(index, conf);
	}*/
	Q_ASSERT(false);
	Log::error(QString("%1.Attempt create child fo BConf.").arg(__CLASS_NAME__));
	return false;
}

bool BConf::insertChild(int index, int typeIndex)
{
	bool rv=false;
	if(childTypesCount())
	{
		QObject* obj = mMetaChildren[typeIndex]->newInstance();
		rv = insertChild(index, static_cast<BConf*>(obj));
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
	return mMetaChildren.count();
}

int BConf::childrenCount() const
{
	return mChildren.count();
}

QStringList BConf::childTypesNames() const
{
	QStringList rv;
	for(const QMetaObject* metaChild : mMetaChildren)
	{
		QString name = tr("No name");
		int i = metaChild->indexOfClassInfo("name");
		if(i>=0)
		{	name = metaChild->classInfo(i).value();	}
		rv << name;
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

void BConf::serialize(QDataStream &out) const
{
	out << mTitle;
}

void BConf::deserialize(QDataStream &in)
{
	QString t;
	in >> t;
	setTitle(t);
}

void BConf::appendMetaChild(const QMetaObject *metaChild)
{
	mMetaChildren << metaChild;
}

bool BConf::event(QEvent *event)
{
	bool rv = QObject::event(event);
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
	return rv;
}
