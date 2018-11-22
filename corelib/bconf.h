#pragma once

#include <QObject>
#include <functional>

/*
проблемы конфигурации:
1. Механизм определения интервалов для источников индикаторов.
2. Каким образом определить может ли родитель содержать потомка данного типа или наоборот.
*/

class BConf : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged USER true)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged USER true)
	typedef std::function<BConf* (BConf* parentConf)> Childctor;
public:
	BConf(BConf* parent = nullptr);
	virtual ~BConf() override;

	const QString& title() const;
	void setTitle(const QString& title);

	const QString& name() const;
	void setName(const QString& name);

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
	void appendChildctor(Childctor childctor);
private:
	QString mTitle;
	QString mName;
	QList<BConf*> mChildren;
	QList<Childctor> mChildCtors;
	QList<BConf*> mChildTypes;
signals:
	void titleChanged(const QString& title);
	void nameChanged(const QString& name);

	// QObject interface
public:
	virtual bool event(QEvent *event) override;
};
