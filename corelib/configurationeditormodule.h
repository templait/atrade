#pragma once

#include <QWidget>

class Configuration;
class ConfigurationModel;
class QModelIndex;

class ConfigurationEditorModule : public QWidget
{
	Q_OBJECT
public:
	ConfigurationEditorModule(const QModelIndex &index, QWidget *parent=nullptr);
	virtual ~ConfigurationEditorModule(){}
protected:
	const QModelIndex& modelIndex() const;
	const Configuration &configuration() const;
	Configuration &configuration();
	ConfigurationModel* model();
private:
	const QModelIndex& mModelIndex;
	ConfigurationModel *mModel;
};
