#include "productconfigurationeditor.h"

ProductConfigurationEditor::ProductConfigurationEditor(const Configuration &configuration, QWidget *parent)
    :QWidget(parent), mConfiguration(configuration)
{

}

const Configuration &ProductConfigurationEditor::configuration() const
{
	return mConfiguration;
}
