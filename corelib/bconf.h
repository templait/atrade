#pragma once

#include <QMap>
#include <QObject>
#include <functional>

/*
проблемы конфигурации:
1. Механизм определения интервалов для источников индикаторов.
2. Каким образом определить может ли родитель содержать потомка данного типа или наоборот.
*/

#define CONF_NAME(name) Q_CLASSINFO("name", name)

class BConf : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged USER true)
public:
	Q_INVOKABLE BConf();
	virtual ~BConf() override;

	const QString& title() const;
	void setTitle(const QString& title);

	template<class ConfT> static bool registerConf();
	template<class ConfT> static bool isConfRegistred();
	static bool isConfRegistred(const BConf * conf);

	BConf* parentConf();
	bool canAppendChild(const BConf* child) const;
	BConf* clone(BConf* parentConf=nullptr) const;
	bool insertChild(int index, BConf* conf);
	bool insertChild(int index, int typeIndex);
	bool appendChild(BConf* conf);
	bool appendChild(int typeIndex);
	int childTypesCount() const;
	int childrenCount() const;
	QStringList childTypesNames() const;
	BConf* childAt(int index);
protected:
	template <class ChildT> void appendChildType();
	virtual void serialize(QDataStream& out) const;
	virtual void deserialize(QDataStream& in);
private:
	void appendMetaChild(const QMetaObject* metaChild);

	QString mTitle;
	QString mName;
	QList<BConf*> mChildren;
	QList<const QMetaObject*> mMetaChildren;
	static QMap<QString, const QMetaObject*> mRegistreadConfs;
signals:
	void titleChanged(const QString& title);

	// QObject interface
public:
	virtual bool event(QEvent *event) override;
};

template <class ChildT>
void BConf::appendChildType()
{
	static_assert(std::is_base_of<BConf, ChildT>::value);
	appendMetaChild(&ChildT::staticMetaObject);
}

template<class ConfT>
bool BConf::isConfRegistred()
{
	static_assert(std::is_base_of<BConf, ConfT>::value);
	return mRegistreadConfs.contains(ConfT::staticMetaObject.className());
}

template<class ConfT>
bool BConf::registerConf()
{
	bool rv = false;
	static_assert(std::is_base_of<BConf, ConfT>::value);
	if(! isConfRegistred<ConfT>())
	{
		mRegistreadConfs[ConfT::staticMetaObject.className()] = &ConfT::staticMetaObject;
		rv = true;
	}
	return rv;
}
