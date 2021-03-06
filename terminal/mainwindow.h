#pragma once

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class LogDoc;
class MDIArea;

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
	MDIArea *currentMDIArea();
	void appendMDIArea(MDIArea *area);
	void onNewChartWindow();
	void onChartWindowConfiguration();
	void onNewTab();
	void onCurrentTabChanfed(int index);

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event) override;
};
