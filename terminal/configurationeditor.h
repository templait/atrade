#pragma once

#include <QDialog>
#include <configuration.h>

class ProductListModel;

namespace Ui
{
class ConfigurationEditor;
}

class ConfigurationEditor : public QDialog
{
	Q_OBJECT
public:
	ConfigurationEditor(const Configuration& configuration, QWidget* parent=nullptr);
	~ConfigurationEditor();
private:
	Ui::ConfigurationEditor *ui;
	Configuration mConfiguration;
	ProductListModel* mProductListModel;
};
