#pragma once

#include <QWidget>

class BConf;

class ConfEditorModule : public QWidget
{
	Q_OBJECT
public:
	ConfEditorModule(BConf &conf, QWidget *parent=nullptr);
	virtual ~ConfEditorModule(){}
protected:
	const BConf &conf() const;
	BConf &conf();
private:
	BConf& mConf;
};
