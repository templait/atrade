#pragma once

#include <QDialog>
#include <configuration.h>

class ProductListModel;
class ConfigurationModel;
class ConfigurationEditorModule;

namespace Ui
{
class ConfigurationEditor;
}

class ConfigurationEditor : public QDialog
{
	Q_OBJECT
public:
	ConfigurationEditor(QWidget* parent=nullptr);
	~ConfigurationEditor() override;
	const Configuration& configuration() const;
private:
	Ui::ConfigurationEditor *ui;
	ProductListModel* mProductListModel;




	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
