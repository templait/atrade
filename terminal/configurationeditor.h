#pragma once

#include <QDialog>
#include <configuration.h>

class ProductListModel;
class ConfigurationModel;
class ProductConfigurationEditor;

namespace Ui
{
class ConfigurationEditor;
}

class ConfigurationEditor : public QDialog
{
	Q_OBJECT
public:
	ConfigurationEditor(const Configuration &configuration, QWidget* parent=nullptr);
	~ConfigurationEditor() override;
	const Configuration& configuration() const;
private:
	Ui::ConfigurationEditor *ui;
	ProductListModel* mProductListModel;
	ConfigurationModel* mConfigurationModel;
	ProductConfigurationEditor* mConfigurationEditor;
	ProductConfigurationEditor* mAppearanceEditor;

	void setConfidurationEditor(ProductConfigurationEditor* configurationEditor);
	void setAppearanceEditor(ProductConfigurationEditor* configurationEditor);
	void setProductEditor(ProductConfigurationEditor* newEditor, ProductConfigurationEditor **dstEditor);
	void onCurrentChanged(const QModelIndex &current, const QModelIndex &);
	void onNewChart();
	void onDelete();

	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
