#pragma once

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class LogDoc;
class QMdiArea;

class MainWindow : public QMainWindow
{
public:
	MainWindow();
	~MainWindow();
private:
	Ui::MainWindow *ui;
	LogDoc *mLogDoc;
	QMdiArea *mMdiArea;

	void initDocks();

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent *event);
};
