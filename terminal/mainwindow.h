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

	void initDocks();
	void saveWindowState() const;
	void loadWindowState();
	void onNewChartWindow();
	void onChartWindowConfiguration();

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event) override;
};
