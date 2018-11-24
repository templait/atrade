#pragma once

#include <QDialog>
#include <configuration.h>

class ProductListModel;
class BConf;
class ConfModel;

namespace Ui
{
class ConfigurationEditor;
}

class ConfigurationEditor : public QDialog
{
	Q_OBJECT
public:
	ConfigurationEditor(BConf* conf, QWidget* parent=nullptr);
	~ConfigurationEditor() override;
	const Configuration& configuration() const;
private:
	Ui::ConfigurationEditor *ui;
	ProductListModel* mProductListModel;
	ConfModel *mConfModel;
	BConf* mConf;

	void execContextMenu(const QPoint& point);

	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
