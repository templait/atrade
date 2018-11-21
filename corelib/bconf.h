#pragma once

#include <QObject>

/*
проблемы конфигурации:
1. Механизм определения интервалов для источников индикаторов.
2. Каким образом определить может ли родитель содержать потомка данного типа или наоборот.
*/

class BConf : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged USER true)
public:
	BConf(BConf* parent = nullptr);
	virtual ~BConf() override {}

	const QString& title() const;
	void setTitle(const QString& title);
	BConf* parentConf();
	virtual bool canAppendChild(const BConf* child) const;
	BConf* clone(BConf* parentConf=nullptr) const;
	bool insertChild(BConf* conf, int index);
	bool appendChild(BConf* conf);
private:
	QString mTitle;
	QList<BConf*> mChildren;
	QList<BConf*> mAvailableTypes;
signals:
	void titleChanged(const QString& title);

	// QObject interface
public:
	virtual bool event(QEvent *event) override;
};
