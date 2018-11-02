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
	Q_OBJECT
public:
	MainWindow();
	~MainWindow() override;
private:
	Ui::MainWindow *ui;
	LogDoc *mLogDoc;
	QMdiArea *mMdiArea;

	void initDocks();
private slots:
	void createChart();

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event) override;
};
