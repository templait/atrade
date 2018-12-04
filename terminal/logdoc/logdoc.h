#pragma once

#include <QDockWidget>

namespace Ui {class wLogDoc;}

class LogModel;

class LogDoc : public QDockWidget
{
	Q_OBJECT
public:
	LogDoc(QWidget *parent=nullptr);
	~LogDoc();
private:
	Ui::wLogDoc *ui;
	LogModel* mLogModel;
};
