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
	ConfigurationEditor(const Configuration &configuration, QWidget* parent=nullptr);
	~ConfigurationEditor() override;
	const Configuration& configuration() const;
private:
	Ui::ConfigurationEditor *ui;
	ProductListModel* mProductListModel;
	ConfigurationModel* mConfigurationModel;
	ConfigurationEditorModule* mConfigurationEditor;
	ConfigurationEditorModule* mAppearanceEditor;
	ConfigurationEditorModule* mTitleConfigurationEditor;

	void setConfidurationEditor(ConfigurationEditorModule* configurationEditor);
	void setAppearanceEditor(ConfigurationEditorModule* configurationEditor);
	void setTitleEditor(ConfigurationEditorModule* configurationEditor);
	void setEditorModule(ConfigurationEditorModule* newEditor, ConfigurationEditorModule **dstEditor);
	void onCurrentChanged(const QModelIndex &current, const QModelIndex &);
	void onNewChart();
	void onDelete();

	// QWidget interface
protected:
	virtual void closeEvent(QCloseEvent *event) override;
};
