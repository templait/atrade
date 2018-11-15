#include "configurationeditormodule.h"
#include <configurationmodel.h>

ConfigurationEditorModule::ConfigurationEditorModule(const QModelIndex& index, QWidget *parent)
	:QWidget(parent), mModelIndex(index)
{
	mModel = const_cast<ConfigurationModel*>(qobject_cast<const ConfigurationModel*>(mModelIndex.model()));
	Q_ASSERT(mModel);
}

const QModelIndex &ConfigurationEditorModule::modelIndex() const
{
	return mModelIndex;
}

const Configuration & ConfigurationEditorModule::configuration() const
{
	return mModel->configuration(mModelIndex);
}

Configuration &ConfigurationEditorModule::configuration()
{
	return mModel->configuration(mModelIndex);
}

ConfigurationModel *ConfigurationEditorModule::model()
{
	return mModel;
}
