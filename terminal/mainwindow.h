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
	void createChart();
	void saveWindowState() const;
	void loadWindowState();

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event) override;
};
