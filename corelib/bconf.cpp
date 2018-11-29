#include "bconf.h"

BConf::BConf(const QString& name) : mParent(nullptr), mName(name)
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
}

const QString &BConf::name() const
{
	return mName;
}

const BConf *BConf::parentConf()
{
	return mParent;
}

bool BConf::appendChild(const BConf& conf)
{
	return insertChild(conf, childrenCount());
}

bool BConf::appendNewChild()
{
	return false;
}

bool BConf::canAppendChild(const BConf &child) const
{
	Q_UNUSED(child)
	return false;
}

bool BConf::insertChild(const BConf &conf, int index)
{
	Q_UNUSED(index)
	Q_UNUSED(conf)
	return false;
}

int BConf::childrenCount() const
{
	return 0;
}

const BConf *BConf::childAt(int index)
{
	Q_UNUSED(index)
	return nullptr;
}

bool BConf::removeChild(int index)
{
	Q_UNUSED(index)
	return false;
}

QString BConf::childName() const
{
	return QString();
}

void BConf::serialize(QDataStream &out) const
{
	out << mName << mTitle;
}

void BConf::deserialize(QDataStream &in)
{
	in >> mName >> mTitle;
}

void BConf::beParentForChild(BConf &child)
{
	child.mParent=this;
}

QDataStream &operator<<(QDataStream &out, const BConf &conf)
{
	conf.serialize(out);
	return out;
}

QDataStream &operator>>(QDataStream &in, BConf &conf)
{
	conf.deserialize(in);
	return in;
}
