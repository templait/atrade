#include "productconfigurationeditor.h"

ProductConfigurationEditor::ProductConfigurationEditor(Configuration *configuration, QWidget *parent)
    :QWidget(parent), mConfiguration(configuration)
{

}

Configuration* ProductConfigurationEditor::configuration() const
{
	return mConfiguration;
}
