#pragma once

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
	typedef std::function<BConf* (BConf* parentConf)> Childctor;
public:
	Q_INVOKABLE BConf(BConf* parent = nullptr);
	virtual ~BConf() override;

	const QString& title() const;
	void setTitle(const QString& title);

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
	template <class ChildT> void appendChildType() {appendMetaChild(&ChildT::staticMetaObject);}
private:
	void appendMetaChild(const QMetaObject* metaChild);

	QString mTitle;
	QString mName;
	QList<BConf*> mChildren;
	QList<const QMetaObject*> mMetaChildren;
signals:
	void titleChanged(const QString& title);

	// QObject interface
public:
	virtual bool event(QEvent *event) override;
};
