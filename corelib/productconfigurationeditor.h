#pragma once

#include <QWidget>

class Configuration;
class ConfigurationModel;
class QModelIndex;

class ProductConfigurationEditor : public QWidget
{
	Q_OBJECT
public:
	ProductConfigurationEditor(const QModelIndex &configuration, QWidget *parent=nullptr);
	virtual ~ProductConfigurationEditor(){}
protected:
	const QModelIndex& modelIndex() const;
	const Configuration &configuration() const;
	Configuration &configuration();
	ConfigurationModel* model();
private:
	const QModelIndex& mModelIndex;
	ConfigurationModel *mModel;
};
