#pragma once

#include <QDialog>
#include <configuration.h>

class ProductListModel;
class ConfigurationModel;

namespace Ui
{
class ConfigurationEditor;
}

class ConfigurationEditor : public QDialog
{
	Q_OBJECT
public:
	ConfigurationEditor(const Configuration& configuration, QWidget* parent=nullptr);
	~ConfigurationEditor() override;
	const Configuration& configuration() const;
private:
	Ui::ConfigurationEditor *ui;
	Configuration mConfiguration;
	ProductListModel* mProductListModel;
	ConfigurationModel* mConfigurationModel;

	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
