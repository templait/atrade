#include "productconfigurationeditor.h"
#include <configurationmodel.h>

ProductConfigurationEditor::ProductConfigurationEditor(const QModelIndex& configuration, QWidget *parent)
	:QWidget(parent), mModelIndex(configuration)
{
	mModel = const_cast<ConfigurationModel*>(qobject_cast<const ConfigurationModel*>(mModelIndex.model()));
	Q_ASSERT(mModel);
}

const QModelIndex &ProductConfigurationEditor::modelIndex() const
{
	return mModelIndex;
}

const Configuration & ProductConfigurationEditor::configuration() const
{
	return mModel->configuration(mModelIndex);
}

Configuration &ProductConfigurationEditor::configuration()
{
	return mModel->configuration(mModelIndex);
}

ConfigurationModel *ProductConfigurationEditor::model()
{
	return mModel;
}
