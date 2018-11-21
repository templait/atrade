#include "bconf.h"

#include <QChildEvent>
#include <QMetaProperty>

BConf::BConf(BConf *parent) : QObject(parent)
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

BConf *BConf::parentConf()
{
	BConf* parentCfg = qobject_cast<BConf*>(parent());
	Q_CHECK_PTR(parentCfg);
	return parentCfg;
}

bool BConf::canAppendChild(const BConf *child) const
{
	Q_UNUSED(child)
	return false;
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

bool BConf::insertChild(BConf *conf, int index)
{
	bool rv = canAppendChild(conf);
	if(rv)
	{
		conf->setParent(this);
		mChildren.insert(index, conf);
	}
	return rv;
}

bool BConf::appendChild(BConf *conf)
{
	return insertChild(conf, mChildren.count());
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
