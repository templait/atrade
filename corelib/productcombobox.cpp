#include "productcombobox.h"
#include "productlistmodel.h"

ProductComboBox::ProductComboBox(QWidget *parent, ProductComboBox::EType type) : QComboBox(parent)
{
	mModel = new ProductListModel(this);
	setType(type);
}

void ProductComboBox::setType(ProductComboBox::EType type)
{
	setModel(mModel);
	switch(type)
	{
	case TypeDataSource:
		setRootModelIndex(mModel->dataSourcesRoot());
		break;
	case TypeIndicator:
		setRootModelIndex(mModel->indicatorsRoot());
		break;
	}
	setEnabled(count());
}

ProductID ProductComboBox::currentProduct() const
{
	ProductID rv;
	if(currentIndex()>=0)
	{
		return rootModelIndex().child(currentIndex(),1).data(Qt::DisplayRole).toUuid();
	}
	return rv;
}
