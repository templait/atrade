#pragma once

#include "chartwindowconf.h"

#include <QDialog>
#include <configuration.h>

class ProductListModel;
class BConf;
class ConfModel;

namespace Ui
{
class ConfEditor;
}

class ConfEditor : public QDialog
{
	Q_OBJECT
public:
	ConfEditor(const ChartWindowConf& conf, QWidget* parent=nullptr);
	~ConfEditor() override;
	const ChartWindowConf& conf() const;
private:
	Ui::ConfEditor *ui;
	ProductListModel* mProductListModel;
	ConfModel *mConfModel;
	ChartWindowConf mConf;

	void clearConfEditor();
	void showChartWindowConf(ChartWindowConf &conf);
	void showChartConf(ChartConf &conf);
	void updateCurrentConf();
	void execContextMenu(const QPoint& point);
	void onCurrentConfChanged(const QModelIndex &index);

	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
